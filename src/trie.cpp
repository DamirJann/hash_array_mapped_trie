#include "../include/trie.h"
#include "../include/utils.h"
#include <bitset>

using namespace std;


bool bitmap::isSet(uint8_t pos) const {
    return (this->data >> pos) % 2;
}

void bitmap::set(uint8_t pos) {
    this->data = this->data | (1 << pos);
}

SNode *createSNode(string &k, int v, uint64_t hash) {
    auto *node = new SNode(k, v, hash);
    node->key = k;
    node->value = v;
    return node;
}


uint8_t CNode::getArrayIndexByBmp(uint8_t pos) const {
    return __builtin_popcount(
            ((1 << pos) - 1) & bmp.data
    );
}

uint8_t SNode::getHashByLevel(uint8_t level) {
    return (hash >> (level * HASH_PIECE_MAX_LEN)) % BRANCH_FACTOR;
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



CNode* createParentWithChild(SNode* child, uint8_t path){
    CNode* parent = new CNode();
    parent->insertChild(child, path);
    return parent;
}

bool Trie::insert(string k, int v) {
    if (root == nullptr) {
        root = new INode(new CNode());
    }

    SNode *subNode = createSNode(k, v, generateHash(k));
    return insert(root, subNode, 0);
}

bool INode::swapToCopyWithReplacedChild(INode *newChild, uint8_t path) {
    CNode *copy = getCopy(this->main);
    copy->replaceChild(newChild, path);
    // cas
    this->main = copy;
    return true;
}

bool Trie::insert(INode *startNode, SNode *newNode, uint8_t level) {
    int path = newNode->getHashByLevel(level);
    Node *subNode = startNode->main->getSubNode(path);

    if (subNode == nullptr) {
        startNode->swapToCopyWithInsertedChild(newNode, path);
        return true;
    } else {
        if (subNode->type == S_NODE) {

            auto * s1 = reinterpret_cast<SNode *>(subNode); // TODO MAKE COPY
            CNode * c1 = createParentWithChild(s1, s1->getHashByLevel(level + 1));
            INode * i1 = new INode(c1);

            startNode->swapToCopyWithReplacedChild(i1, s1->getHashByLevel(level));
        }

        return insert(static_cast<INode*>(startNode->main->getSubNode(path)), newNode, level + 1);
    }
}


