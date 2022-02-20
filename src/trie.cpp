#include "../include/trie.h"
#include "../include/utils.h"
#include <bitset>

using namespace std;


bool bitmap::is_set(uint8_t pos) const {
    return (this->data >> pos) % 2;
}

void bitmap::set(uint8_t pos) {
    this->data = this->data | (1 << pos);
}

uint64_t get_hash(string k) {
    uint64_t hash = 0;
    for (size_t i = 0; i < k.size(); i++) {
        hash +=  k[i] * i;
    }
    return hash % 10;
}

SNode *create_s_node(string k, int v) {
    auto *node = new SNode();
    node->key = k;
    node->value = v;
    return node;
}


uint8_t CNode::get_array_index_by_bmp(uint8_t pos) const {
//    return __popcount(
//            ((1 << pos) - 1) & bmp.data
//    );
    return 0 + pos;
}

void CNode::addNode(SNode *node) {
    vector<Node *> upd_arr;
    uint8_t i = 0;
    while (i < BRANCH_FACTOR) {
        if (!bmp.is_set(i)) {
            bmp.set(1);
            upd_arr.push_back(node);
        } else {
            int ind = this->get_array_index_by_bmp(i);
            upd_arr.push_back(array[ind]);
        }
        i++;
    }
}

Node *CNode::getNode(uint8_t path) {
    if (!bmp.is_set(path)) return nullptr;
    int index = this->get_array_index_by_bmp(path);
    return array[index];
}

void CNode::replace_child_s_node_to_c_node(int pos) {
    SNode *child_node = static_cast<SNode *>(getNode(pos));
    CNode *c_node = new CNode(child_node);
    int index = get_array_index_by_bmp(pos);
    array[index] = c_node;
}

bool Trie::insert(string k, int v) {
    if (root == nullptr) {
        root = new CNode();
    }
    return insert(get_hash(k), k, v, reinterpret_cast<CNode *>(root), 1);
}

bool Trie::insert(uint64_t hash, string k, int v, CNode *node, uint8_t level) {
    int path = get_path_by_level(hash, level);

    if (node->getNode(path) != nullptr) {
        node->addNode(create_s_node(k, v));
        return true;
    } else {
        node->replace_child_s_node_to_c_node(path);
        return insert(hash, k, v, static_cast<CNode *>(node->getNode(path)), level + 1);
    }
}
