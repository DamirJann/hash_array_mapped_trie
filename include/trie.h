#include <cstdint>
#include <string>
#include <vector>
#include <bitset>
#include "utils.h"
#include <set>
#include <mutex>
#include "atomic"

#pragma once
using namespace std;


struct bitmap {
    uint32_t data;

    bool isSet(uint8_t pos) const {
        return (this->data >> pos) % 2;
    }

    void set(uint8_t pos) {
        this->data = this->data | (1 << pos);
    }

    void unset(uint8_t pos) {
        this->data = this->data & (~(1 << pos));
    }
};

enum NodeType {
    CNODE,
    SNODE,
    INODE
};

class Node {
public:
    NodeType type;
protected:

    Node(NodeType type) {
        this->type = type;
    }
};

template<class K, class V>
struct Pair {
    K key;
    V value;

    bool operator<(const Pair &p) const {
        return this->key < p.key;
    }
};

template<class K, class V>
class SNode : public Node {
public:

    SNode(K k, V v) : Node(SNODE) {
        this->pair.insert({k, v});
        this->hash = generateSimpleHash(k);
    }

    uint64_t getHash() {
        return hash;
    }

    bool contains(K k) {
        for (auto &p: this->pair) {
            if (p.key == k) {
                return true;
            }
        }
        return false;
    }

    bool contains(SNode<K, V> *node) {
        for (auto &p: node->pair) {
            if (this->contains(p.key)) {
                return true;
            }
        }
        return false;
    }

    int getValue(K k) {
        for (auto &p: this->pair) {
            if (p.key == k) {
                return p.value;
            }
        }
        return 0;
    }


    void merge(SNode<K, V> *node) {
        for (auto &p: node->pair) {
            this->pair.insert(p);
        }
    }

public:
    uint64_t hash{};
    set<Pair<K, V>> pair;
};

template<class K, class V>
SNode<K, V> *leftMerge(SNode<K, V> *node1, SNode<K, V> *node2) {
    auto *merged = new SNode<K, V>(*node1);
    for (auto &p: node2->pair) {
        if (!merged->contains(p.key)) {
            merged->pair.insert(p);
        }
    }
    return merged;
}

template<class K, class V>
SNode<K, V> *createSNode(K &k, V v, uint64_t hash) {
    return new SNode<K, V>(k, v, hash);
}

template<class K, class V>
class CNode : public Node {
public:
    friend class TestCNode;

    CNode() : Node(CNODE) {
        bmp = {0};
    }


    Node *getSubNode(uint8_t path) {
        if (!bmp.isSet(path)) return nullptr;
        int index = this->getArrayIndexByBmp(path);
        return array[index];
    }

    uint8_t getChildCount() {
        return this->array.size();
    }

    Node *getFirst() {
        return this->array.front();
    }

    void insertChild(Node *newChild, uint8_t path) {
        this->bmp.set(path);
        this->array.insert(this->array.begin() + this->getArrayIndexByBmp(path), newChild);
    }

    void replaceChild(Node *newChild, uint8_t path) {
        this->array[this->getArrayIndexByBmp(path)] = newChild;
    }

    void deleteChild(uint8_t path) {
        array.erase(array.begin() + getArrayIndexByBmp(path));
        bmp.unset(path);
    }


private:
    bitmap bmp;
    vector<Node *> array;

    uint8_t getArrayIndexByBmp(uint8_t pos) const {
        return __builtin_popcount(
                ((1 << pos) - 1) & bmp.data
        );
    }
};


template<class K, class V>
class INode : public Node {
public:
    INode() : Node(INODE) {}

    INode(CNode<K, V> *main) : INode() {
        this->main.store(main, std::memory_order_seq_cst);
    }

    bool swapToCopyWithReplacedChild(Node *newChild, uint8_t path) {
        CNode<K, V> *copy = getCopy(waitMain());
        copy->replaceChild(newChild, path);
        // cas
        this->main = copy;
        return true;
    }

    bool swapToCopyWithDeletedKey(K key, uint8_t path) {
        CNode<K, V> *copy = getCopy(waitMain());
        SNode<K, V> *subNode = static_cast<SNode<K, V> *>(copy->getSubNode(path));

        for (auto &p: subNode->pair) {
            if (p.key == key) {
                subNode->pair.erase(p);
                this->main = copy;
                return true;
            }
        }

        return false;
    }

    CNode<K, V> *waitMain() {
        CNode<K, V> *res = nullptr;
        while (res == nullptr) {
            res = this->main.load();
        }
        return res;
    }


    atomic<CNode<K, V> *> main;
};

template<class K, class V>
bool tryToContract(INode<K, V> *currentNode, INode<K, V> *reducedNode, uint8_t path) {
    if (currentNode == nullptr) return false;

    switch (reducedNode->waitMain()->getChildCount()) {
        case 0: {
            return true;
        }
        case 1: {
            Node *replaced = reducedNode->waitMain()->getFirst();
            currentNode->swapToCopyWithReplacedChild(replaced, path);
            return true;
        }
        default: {
            return false;
        }
    }
}

enum LookupResultState {
    NotFound,
    Found,
    Failed
};


struct LookupResult {
    int value;
    LookupResultState state;
};

bool operator==(const LookupResult a, const LookupResult b) {
    if ((a.state == NotFound) && (b.state == NotFound)) return true;
    if ((a.state == Failed) && (b.state == Failed)) return true;
    if ((a.state == Found) && (b.state == Found)) return a.value == b.value;
    return false;
}


bool operator!=(const LookupResult a, const LookupResult b) {
    return !(a == b);
}

LookupResult createSuccessfulLookupResult(int value) {
    return {value, Found};
}

const LookupResult NOT_FOUND{0, NotFound};
const LookupResult RESTART{0, Failed};


template<class K, class V>
CNode<K, V> *getCopy(CNode<K, V> *node) {
    return new CNode<K, V>(*node);
}


// i1 -> c1 -> s1, ...
// add s2
// c1' -> s1, ...
// c1' -> (new s1 + s2), ...
//template<class K, class V>
//CNode<K, V> *buildCopyWithMergedChild(CNode<K, V> *org, SNode<K, V> *subNode, SNode<K, V> *newNode, uint8_t path) {
//    CNode<K, V> *copy = getCopy(org);
//    copy->replaceChild(leftMerge(subNode, newNode), path);
//    return copy;
//}

template<class K, class V>
CNode<K, V> *buildCopyWithReplacedPair(CNode<K, V> *copy, SNode<K, V> *subNode, SNode<K, V> *newNode, uint8_t path) {
    copy->replaceChild(leftMerge(newNode, subNode), path);
    return copy;
}

template<class K, class V>
CNode<K, V> *buildCopyWithMergedChild(CNode<K, V> *copy, SNode<K, V> *subNode, SNode<K, V> *newNode, uint8_t path) {
    copy->replaceChild(leftMerge(newNode, subNode), path);
    return copy;
}

template<class K, class V>
CNode<K, V> *
buildCopyWithDownChild(CNode<K, V> *c1, SNode<K, V> *child2, SNode<K, V> *child3, uint8_t level, uint8_t path) {
    auto *c2 = new CNode<K, V>();
    c2->insertChild(child2, extractHashPartByLevel(child2->getHash(), level + 1));
    c2->insertChild(child3, extractHashPartByLevel(child3->getHash(), level + 1));
    auto *i2 = new INode<K, V>(c2);
    c1->replaceChild(i2, path);
    return c1;
}


template<class K, class V>
CNode<K, V> *buildCNodeWithSNodeChild(SNode<K, V> *child, uint8_t path) {
    CNode<K, V> *parent = new CNode<K, V>();
    parent->insertChild(child, path);
    return parent;
}


template<class K, class V>
class Trie {
private:
    INode<K, V> *root;
    mutex mx;
public:
    Trie() {
        root = new INode<K, V>();
    }

    Node *getRoot() {
        return this->root;
    }

    LookupResult lookup(K k) {
        while (true) {
            if (root->main.load() == nullptr) {
                return NOT_FOUND;
            } else {
                LookupResult r = lookup(root, k, generateSimpleHash(k), 0);
                if (r != RESTART) {
                    return r;
                }
            }
        }
    }


    bool remove(K key) {
        mx.lock();

        if (root == nullptr) {
            root = new INode<K, V>(new CNode<K, V>());
        }
        bool res = this->remove(nullptr, root, key, generateSimpleHash(key), 0);
        mx.unlock();
        return res;
    }


    bool insert(K key, V value) {
        while (true) {
            CNode<K, V> *old = root->main.load();
            if (old == nullptr) {
                // root -> c -> s
                auto *c = new CNode<K, V>();
                auto *s = new SNode<K, V>(key, value);
                c->insertChild(s, extractHashPartByLevel(s->getHash(), 0));
                if (root->main.compare_exchange_strong(old, c)) {
                    return true;
                }
            } else {
                if (insert(root, new SNode<K, V>(key, value), 0)) {
                    return true;
                }
            }
        }
    }


private:
    LookupResult lookup(INode<K, V> *startNode, K key, uint64_t hash, uint8_t level) {
        Node *nextNode = startNode->main.load()->getSubNode(extractHashPartByLevel(hash, level));
        if (nextNode == nullptr) {
            return NOT_FOUND;
        } else if (nextNode->type == SNODE) {
            if (static_cast<SNode<K, V> *>(nextNode)->contains(key)) {
                return createSuccessfulLookupResult(static_cast<SNode<K, V> *>(nextNode)->getValue(key));
            }
            return NOT_FOUND;
        } else if (nextNode->type == INODE) {
            return lookup(static_cast<INode<K, V> *>(nextNode), key, hash, level + 1);
        }
    }

    bool remove(INode<K, V> *iParent, INode<K, V> *currentNode, K key, uint64_t hash, uint8_t level) {
        Node *subNode = currentNode->waitMain()->getSubNode(extractHashPartByLevel(hash, level));

        if (subNode == nullptr) return false;

        switch (subNode->type) {
            case SNODE: {
                if (!static_cast<SNode<K, V> *>(subNode)->contains(key)) {
                    return false;
                }
                currentNode->swapToCopyWithDeletedKey(key, extractHashPartByLevel(hash, level));
                break;
            }
            case INODE: {
                if (!this->remove(currentNode, static_cast<INode<K, V> *>(subNode), key, hash, level + 1)) {
                    return false;
                }
                break;
            }
            default: {
                static_assert(true, "Trie is build wrong");
                return false;
            }
        }

        tryToContract(iParent, currentNode, extractHashPartByLevel(hash, level - 1));
        return true;
    }

    bool insert(INode<K, V> *startNode, SNode<K, V> *newNode, uint8_t level) {
        CNode<K, V> *exp = startNode->main.load();
        // TODO don't copy if subNode is INODE
        CNode<K, V> *orgCopy = getCopy(exp);

        int path = extractHashPartByLevel(newNode->getHash(), level);
        Node *subNode = orgCopy->getSubNode(path);

        if (subNode == nullptr) {
            CNode<K, V> *updated = orgCopy;
            updated->insertChild(newNode, path);
            if (startNode->main.compare_exchange_strong(exp, updated)) {
                return true;
            }
            return false;
        } else if (subNode->type == SNODE) {
            auto *sSubNode = static_cast<SNode<K, V> *>(subNode);
            if (sSubNode->contains(newNode)) {
                CNode<K, V> *updated = buildCopyWithReplacedPair(orgCopy, sSubNode, newNode, path);
                return startNode->main.compare_exchange_strong(exp, updated);
            } else if (level == 12) {
                CNode<K, V> *updated = buildCopyWithMergedChild(orgCopy, sSubNode, newNode, path);
                return startNode->main.compare_exchange_strong(exp, updated);
            } else {
                CNode<K, V> *updated = buildCopyWithDownChild<K, V>(orgCopy, newNode, sSubNode, level, path);
                startNode->main.compare_exchange_strong(exp, updated);
                return false;
            }
        } else if (subNode->type == INODE) {
            auto *next = static_cast<INode<K, V> *>(startNode->waitMain()->getSubNode(path));
            return insert(next, newNode, level + 1);
        } else {
            assert(false);
            return false;
        }
    }


};

