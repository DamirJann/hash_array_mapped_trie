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
        CNode<K, V> *copy = getCopy(main.load());
        copy->replaceChild(newChild, path);
        // cas
        this->main = copy;
        return true;
    }

    atomic<CNode<K, V> *> main;
};


struct LookupResult {
    enum LookupResultStatus {
        NotFound,
        Found,
        Failed
    };

    int value;
    LookupResultStatus status;

    bool operator==(LookupResult b) const {
        if ((this->status == NotFound) && (b.status == NotFound)) return true;
        if ((this->status == Failed) && (b.status == Failed)) return true;
        if ((this->status == Found) && (b.status == Found)) return this->value == b.value;
        return false;
    }

    bool operator!=(LookupResult b) const {
        return !(*this == b);
    }
};

LookupResult createSuccessfulLookupResult(int value) {
    return {value, LookupResult::Found};
}

const LookupResult LOOKUP_NOT_FOUND{0, LookupResult::NotFound};
const LookupResult LOOKUP_RESTART{0, LookupResult::Failed};

struct RemoveResult {
    enum Status {
        Removed,
        Failed,
        NotFound,
    };

    int value;
    Status status;

    bool operator==(RemoveResult rightOperand) const {
        if ((this->status == NotFound) && (rightOperand.status == NotFound)) return true;
        if ((this->status == Failed) && (rightOperand.status == Failed)) return true;
        if ((this->status == Removed) && (rightOperand.status == Removed)) return this->value == rightOperand.value;
        return false;
    }

    bool operator!=(RemoveResult rightOperand) const {
        return !(*this == rightOperand);
    }
};

const RemoveResult REMOVE_NOT_FOUND{0, RemoveResult::NotFound};
const RemoveResult REMOVE_RESTART{0, RemoveResult::Failed};

RemoveResult createSuccessfulRemoveResult(int value) {
    return {value, RemoveResult::Removed};
}

template<class K, class V>
CNode<K, V> *getCopy(CNode<K, V> *node) {
    return new CNode<K, V>(*node);
}

template<class K, class V>
void transformToContractedParent(CNode<K, V> *updated, CNode<K, V> *m, uint8_t path) {
    updated->replaceChild(m->getFirst(), path);
}

template<class K, class V>
void transformToWithReplacedPair(CNode<K, V> *updated, SNode<K, V> *subNode, SNode<K, V> *newNode, uint8_t path) {
    updated->replaceChild(leftMerge(newNode, subNode), path);
}

template<class K, class V>
void transformToWithMergedChild(CNode<K, V> *updated, SNode<K, V> *subNode, SNode<K, V> *newNode, uint8_t path) {
    updated->replaceChild(leftMerge(newNode, subNode), path);
}

template<class K, class V>
void
transformToWithDownChild(CNode<K, V> *updated, SNode<K, V> *child2, SNode<K, V> *child3, uint8_t level, uint8_t path) {
    auto *c2 = new CNode<K, V>();
    c2->insertChild(child2, extractHashPartByLevel(child2->getHash(), level + 1));
    c2->insertChild(child3, extractHashPartByLevel(child3->getHash(), level + 1));
    auto *i2 = new INode<K, V>(c2);
    updated->replaceChild(i2, path);
}

template<class K, class V>
void transformToWithDeletedKey(CNode<K, V> *updated, SNode<K, V> *subNode, K key, uint8_t path) {
    auto *newSubNode = new SNode<K, V>(*subNode);
    newSubNode->pair.erase({key, subNode->getValue(key)});
    updated->replaceChild(newSubNode, path);
}


template<class K, class V>
void contractParent(INode<K, V> *parent, INode<K, V> *i, uint8_t path) {
    while (true) {
        CNode<K, V> *old = parent->main.load();
        CNode<K, V> *updated = getCopy(old);
        CNode<K, V> *m = i->main.load();

        if (old->getChildCount() == 1) {
            transformToContractedParent(updated, m, path);
            if (parent->main.compare_exchange_strong(old, updated)) {
                break;
            }
        } else {
            break;
        }
    }
}

template<class K, class V>
class Trie {
private:
    INode<K, V> *root;
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
                return LOOKUP_NOT_FOUND;
            } else {
                LookupResult res = lookup(root, k, generateSimpleHash(k), 0);
                if (res != LOOKUP_RESTART) {
                    return res;
                }
            }
        }
    }

    RemoveResult remove(K key) {
        while (true) {
            if (root->main.load() == nullptr) {
                return REMOVE_NOT_FOUND;
            }
            RemoveResult res = this->remove(root, nullptr, key, generateSimpleHash(key), 0);
            if (res != REMOVE_RESTART) {
                return res;
            }
        }
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
            return LOOKUP_NOT_FOUND;
        } else if (nextNode->type == SNODE) {
            if (static_cast<SNode<K, V> *>(nextNode)->contains(key)) {
                return createSuccessfulLookupResult(static_cast<SNode<K, V> *>(nextNode)->getValue(key));
            }
            return LOOKUP_NOT_FOUND;
        } else if (nextNode->type == INODE) {
            return lookup(static_cast<INode<K, V> *>(nextNode), key, hash, level + 1);
        }
    }

    RemoveResult remove(INode<K, V> *currentNode, INode<K, V> *parent, K key, uint64_t hash, uint8_t level) {
        CNode<K, V> *old = currentNode->main.load();
        CNode<K, V> *updated = getCopy(old);

        uint8_t path = extractHashPartByLevel(hash, level);
        Node *subNode = updated->getSubNode(path);

        RemoveResult res{};

        if (subNode == nullptr) {
            res = REMOVE_NOT_FOUND;
        } else if (subNode->type == SNODE) {
            if (!static_cast<SNode<K, V> *>(subNode)->contains(key)) {
                res = REMOVE_NOT_FOUND;
            } else {
                V deletedValue = static_cast<SNode<K, V> *>(subNode)->getValue(key);
                transformToWithDeletedKey(updated, static_cast<SNode<K, V> *>(subNode), key,
                                          extractHashPartByLevel(hash, level));
                res = (currentNode->main.compare_exchange_strong(old, updated))
                      ? createSuccessfulRemoveResult(deletedValue) : REMOVE_RESTART;
            }
        } else if (subNode->type == INODE) {
            res = this->remove(static_cast<INode<K, V> *>(subNode), static_cast<INode<K, V> *>(subNode), key, hash,
                               level + 1);
        }

        if (res == REMOVE_NOT_FOUND || res == REMOVE_RESTART) {
            return res;
        }

        if (parent != nullptr) {
            contractParent(parent, currentNode, extractHashPartByLevel(hash, level - 1));
        }

        return res;
    }

    bool insert(INode<K, V> *currentNode, SNode<K, V> *newNode, uint8_t level) {
        CNode<K, V> *old = currentNode->main.load();
        CNode<K, V> *updated = getCopy(old);

        uint8_t path = extractHashPartByLevel(newNode->getHash(), level);
        Node *subNode = updated->getSubNode(path);

        if (subNode == nullptr) {
            updated->insertChild(newNode, path);
            return currentNode->main.compare_exchange_strong(old, updated);
        } else if (subNode->type == SNODE) {
            auto *sSubNode = static_cast<SNode<K, V> *>(subNode);
            if (sSubNode->contains(newNode)) {
                transformToWithReplacedPair(updated, sSubNode, newNode, path);
                return currentNode->main.compare_exchange_strong(old, updated);
            } else if (level == MAX_LEVEL_COUNT) {
                transformToWithMergedChild(updated, sSubNode, newNode, path);
                return currentNode->main.compare_exchange_strong(old, updated);
            } else {
                transformToWithDownChild<K, V>(updated, newNode, sSubNode, level, path);
                return currentNode->main.compare_exchange_strong(old, updated);
            }
        } else if (subNode->type == INODE) {
            return insert(static_cast<INode<K, V> *>(subNode), newNode, level + 1);
        } else {
            assert(false);
            return false;
        }
    }
};

