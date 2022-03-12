#include <cstdint>
#include <string>
#include <vector>
#include <bitset>
#include "utils.h"
#include <set>
#pragma once
using namespace std;


struct bitmap {
    uint32_t data;

    bool isSet(uint8_t pos) const {
        return (this->data >> pos) % 2;
    }

    void set(uint8_t pos){   this->data = this->data | (1 << pos);
    }

    void unset(uint8_t pos) {
        this->data = this->data & (~(1 << pos));
    }
};

enum NodeType {
    C_NODE,
    S_NODE,
    I_NODE
};

class Node {
public:
    NodeType type;
protected:
    Node(NodeType type) {
        this->type = type;
    }
};

template <class K, class V>
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

    SNode(K k, V v, uint64_t hash) : Node(S_NODE) {
        this->pair.insert({k, v});
        this->hash = hash;
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
    uint64_t hash;
    set<Pair<K, V>> pair;
};

template<class K, class V>
SNode<K, V> *createSNode(K &k, V v, uint64_t hash) {
    return new SNode<K, V>(k, v, hash);
}

class CNode : public Node {
public:
    friend class TestCNode;

    CNode() : Node(C_NODE) {
        bmp = {0};
    }

    CNode(CNode *node) : CNode() {
        this->bmp.data = node->bmp.data;
        this->array = node->array;
    }

    Node *getSubNode(uint8_t path) {
        if (!bmp.isSet(path)) return nullptr;
        int index = this->getArrayIndexByBmp(path);
        return array[index];
    }

    bool containsTheOnlyKey() {
        return this->array.size() == 1;
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
        array.erase(array.begin() + this->getArrayIndexByBmp(path));
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

CNode *getCopy(CNode *node) {
    return new CNode(*node);
}

template<class K, class V>
class INode : public Node {
public:
    INode(CNode *main) : Node(I_NODE) {
        this->main = main;
    }

    bool swapToCopyWithReplacedChild(Node *newChild, uint8_t path) {
        CNode *copy = getCopy(this->main);
        copy->replaceChild(newChild, path);
        // cas
        this->main = copy;
        return true;
    }


    bool swapToCopyWithInsertedChild(Node *child, uint8_t path) {
        CNode *copy = getCopy(this->main);
        copy->insertChild(child, path);
        // cas
        this->main = copy;
        return true;
    }


    bool swapToCopyWithDeletedChild(uint8_t path) {
        CNode *copy = getCopy(this->main);
        copy->deleteChild(path);
        // cas
        this->main = copy;
        return true;
    }

    void swapToCopyWithDownChild(Node *child, uint8_t level) {
        auto *s1 = reinterpret_cast<SNode<K, V> *>(child);
        CNode *c1 = createParentWithChild(s1, extractHashPartByLevel(s1->getHash(), level + 1));
        INode *i1 = new INode(c1);
        this->swapToCopyWithReplacedChild(i1, extractHashPartByLevel(s1->getHash(), level));
    }

    CNode *main;
};

struct LookupResult {
    int value;
    bool isFound;
};

LookupResult createLookupResult(int value) {
    return {value, true};
}

const LookupResult NOT_FOUND{0, false};


template<class K, class V>
bool tryToContract(INode<K, V> *currentNode, INode<K, V> *reducedNode, uint8_t path) {
    if (currentNode == nullptr || !reducedNode->main->containsTheOnlyKey()) return false;

    Node *replaced = reducedNode->main->getFirst();
    currentNode->swapToCopyWithReplacedChild(replaced, path);

    return true;
}

template<class K, class V>
class Trie {
private:
    INode<K, V> *root;
public:

    Trie() {
        root = new INode<K, V>(new CNode());
    }

    Node *getRoot() {
        return this->root;
    }

    LookupResult lookup(K k) {
        if (root == nullptr) {
            root = new INode<K, V>(new CNode());
        }
        return lookup(root, k, generateHash(k), 0);
    }

    bool remove(K key) {
        if (root == nullptr) {
            root = new INode<K, V>(new CNode());
        }
        return this->remove(nullptr, root, key, generateHash(key), 0);
    }


    bool insert(K key, V value) {
        if (root == nullptr) {
            root = new INode<K, V>(new CNode());
        }
        SNode<K, V> *subNode = createSNode(key, value, generateHash(key));
        return insert(root, subNode, 0);
    }


private:
    LookupResult lookup(INode<K, V> *startNode, K key, uint64_t hash, uint8_t level) {
        Node *nextNode = startNode->main->getSubNode(extractHashPartByLevel(hash, level));

        if (nextNode == nullptr) return NOT_FOUND;

        switch (nextNode->type) {
            case S_NODE: {
                if (static_cast<SNode<K, V> *>(nextNode)->contains(key)) {
                    return createLookupResult(static_cast<SNode<K, V> *>(nextNode)->getValue(key));
                }
                return NOT_FOUND;
            }
            case I_NODE: {
                return lookup(static_cast<INode<K, V> *>(nextNode), key, hash, level + 1);
            }
            default: {
                static_assert(true, "Trie is build wrong");
                return NOT_FOUND;
            }
        }
    }

    bool remove(INode<K, V> *iParent, INode<K, V> *currentNode, K key, uint64_t hash, uint8_t level) {
        Node *subNode = currentNode->main->getSubNode(extractHashPartByLevel(hash, level));

        if (subNode == nullptr) return false;

        switch (subNode->type) {
            case S_NODE: {
                if (static_cast<SNode<K, V> *>(subNode)->contains(key)) {
                    currentNode->swapToCopyWithDeletedChild(extractHashPartByLevel(hash, level));
                }
                break;
            }
            case I_NODE: {
                while (!this->remove(currentNode, static_cast<INode<K, V> *>(subNode), key, hash, level + 1));
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

    static bool insert(INode<K, V> *startNode, SNode<K, V> *newNode, uint8_t level) {
        int path = extractHashPartByLevel(newNode->getHash(), level);
        Node *subNode = startNode->main->getSubNode(path);

        if (subNode == nullptr) {
            startNode->swapToCopyWithInsertedChild(newNode, path);
            return true;
        } else {
            if (subNode->type == S_NODE) {
                if (level == 12 || static_cast<SNode<K, V> *>(subNode)->contains(newNode)) {
                    newNode->merge(static_cast<SNode<K, V> *>(subNode));
                    startNode->swapToCopyWithReplacedChild(newNode, path);
                    return true;
                } else {
                    startNode->swapToCopyWithDownChild(subNode, level);
                }
            }
            return insert(static_cast<INode<K, V> *>(startNode->main->getSubNode(path)), newNode, level + 1);
        }
    }

};

template<class K, class V>
CNode *createParentWithChild(SNode<K, V> *child, uint8_t path) {
    CNode *parent = new CNode();
    parent->insertChild(child, path);
    return parent;
}
