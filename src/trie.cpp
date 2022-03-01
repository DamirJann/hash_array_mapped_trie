#include "../include/trie.h"
#include "../include/utils.h"

using namespace std;


bool bitmap::isSet(uint8_t pos) const {
    return (this->data >> pos) % 2;
}

void bitmap::set(uint8_t pos) {
    this->data = this->data | (1 << pos);
}

SNode *createSNode(string &k, int v, uint64_t hash) {
    return new SNode(k, v, hash);
}

uint8_t CNode::getArrayIndexByBmp(uint8_t pos) const {
    return __builtin_popcount(
            ((1 << pos) - 1) & bmp.data
    );
}

CNode *getCopy(CNode *node) {
    return new CNode(*node);
}

bool INode::swapToCopyWithInsertedChild(Node *child, uint8_t path) {
    CNode *copy = getCopy(this->main);
    copy->insertChild(child, path);
    // cas
    this->main = copy;
    return true;
}


Node *CNode::getSubNode(uint8_t path) {
    if (!bmp.isSet(path)) return nullptr;
    int index = this->getArrayIndexByBmp(path);
    return array[index];
}

void CNode::insertChild(Node *newChild, uint8_t path) {
    this->bmp.set(path);
    this->array.insert(this->array.begin() + this->getArrayIndexByBmp(path), newChild);
}

void CNode::replaceChild(Node *newChild, uint8_t path) {
    this->array[this->getArrayIndexByBmp(path)] = newChild;
}


CNode *createParentWithChild(SNode *child, uint8_t path) {
    CNode *parent = new CNode();
    parent->insertChild(child, path);
    return parent;
}

Node *Trie::getRoot() {
    return this->root;
}

bool Trie::insert(string k, int v) {
    if (root == nullptr) {
        root = new INode(new CNode());
    }

    SNode *subNode = createSNode(k, v, generateHash(k));
    return insert(root, subNode, 0);
}

bool INode::swapToCopyWithReplacedChild(Node *newChild, uint8_t path) {
    CNode *copy = getCopy(this->main);
    copy->replaceChild(newChild, path);
    // cas
    this->main = copy;
    return true;
}

bool INode::tryToContract(uint8_t path) {

    // TODO not implemented
    return path;
}

bool Trie::insert(INode *startNode, SNode *newNode, uint8_t level) {
    int path = extractHashPartByLevel(newNode->getHash(), level);
    Node *subNode = startNode->main->getSubNode(path);

    if (subNode == nullptr) {
        startNode->swapToCopyWithInsertedChild(newNode, path);
        return true;
    } else {
        if (subNode->type == S_NODE && level == 12) {
            newNode->merge(static_cast<SNode *>(subNode));
            startNode->swapToCopyWithReplacedChild(newNode, path);
            return true;
        } else if (subNode->type == S_NODE) {
            auto *s1 = reinterpret_cast<SNode *>(subNode);
            CNode *c1 = createParentWithChild(s1, extractHashPartByLevel(s1->getHash(), level + 1));
            INode *i1 = new INode(c1);

            startNode->swapToCopyWithReplacedChild(i1, extractHashPartByLevel(s1->getHash(), level));
        }

        return insert(static_cast<INode *>(startNode->main->getSubNode(path)), newNode, level + 1);
    }
}

LookupResult Trie::lookup(string k) {
    if (root == nullptr) {
        root = new INode(new CNode());
    }
    return lookup(root, k, generateHash(k), 0);
}


LookupResult Trie::lookup(INode *startNode, string k, uint64_t hash, uint8_t level) {
    Node *nextNode = startNode->main->getSubNode(extractHashPartByLevel(hash, level));

    if (nextNode == nullptr) return NOT_FOUND;

    switch (nextNode->type) {
        case S_NODE: {
            if (static_cast<SNode *>(nextNode)->contains(k)) {
                return createLookupResult(static_cast<SNode *>(nextNode)->getValue(k));
            }
            return NOT_FOUND;
        }
        case I_NODE: {
            return lookup(static_cast<INode *>(nextNode), k, hash, level + 1);
        }
        default: {
            static_assert(true, "Trie is build wrong");
            return NOT_FOUND;
        }
    }
}

bool Trie::remove(string k) {
    if (root == nullptr) {
        root = new INode(new CNode());
    }
    return remove(root, k, generateHash(k), 0);
}

bool Trie::remove(INode *startNode, string k, uint64_t hash, uint8_t level) {
    Node *nextNode = startNode->main->getSubNode(extractHashPartByLevel(hash, level));

    if (nextNode == nullptr) return false;

    switch (nextNode->type) {
        case S_NODE: {
            if (static_cast<SNode *>(nextNode)->contains(k)) {
                startNode->swapToCopyWithReplacedChild(nullptr, extractHashPartByLevel(hash, level));
                startNode->tryToContract(extractHashPartByLevel(hash, level));
                return true;
            }
        }
        case I_NODE: {
            return remove(static_cast<INode *>(nextNode), k, hash, level + 1);
        }
        default: {
            static_assert(true, "Trie is build wrong");
            return false;
        }
    }
}


LookupResult createLookupResult(int value) {
    return {value, true};
}

uint64_t SNode::getHash() {
    return hash;
}

bool SNode::contains(string k) {
    for (auto &p: this->pair) {
        if (p.key == k) {
            return true;
        }
    }
    return false;
}

void SNode::merge(SNode *node) {
    for (auto &p: node->pair) {
        this->pair.insert(p);
    }
}

int SNode::getValue(string k) {
    for (auto &p: this->pair) {
        if (p.key == k) {
            return p.value;
        }
    }
    return 0;
}

