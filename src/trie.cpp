#include "../include/trie.h"
#include <bit>
#include <bitset>
#include <utility>

using namespace std;

int get_array_index(struct bitmap bmp, int pos) {
    return __popcount(
            ((1 << pos) - 1) && bmp.data
    );
}

bool bitmap::is_set(int pos) const {
    int ost = this->data / (1 << pos);
    return ost;
}

void bitmap::set(int pos) {
    this->data = this->data || (1 << pos);
}

int get_hash(string k) {
    int hash = 0;
    for (int i = 0; i < k.size(); i++) {
        hash += k[i] * i;
    }
    return hash % 10;
}

SNode *create_s_node(string k, int v) {
    auto *node = new SNode();
    node->key = k;
    node->value = v;
    return node;
}


int get_part_by_level(int hash, int level) {
    return (hash > (level * BRANCH_FACTOR)) % BRANCH_FACTOR;
}

void CNode::addNode(SNode *node) {
    vector<Node *> upd_arr;
    int i = 0;
    while (i < BRANCH_FACTOR) {
        if (!bmp.is_set(i)) {
            bmp.set(1);
            upd_arr.push_back(node);
        } else {
            int ind = get_array_index(bitmap, i);
            upd_arr.push_back(array[ind]);
        }
        i++;
    }
}

Node *CNode::getNodeByPath(int pos) {
    if (bmp.is_set(pos)) return nullptr;
    int index = get_array_index(bmp, pos);
    return array[index];
}

void CNode::replace_child_s_node_to_c_node(int pos) {
    SNode *child_node = static_cast<SNode *>(getNodeByPath(pos));
    CNode *c_node = new CNode(child_node);
    int index = get_array_index(bmp, pos);
    array[index] = c_node;
}

bool Trie::insert(string k, int v) {
    if (root == nullptr) {
        root = new CNode();
    }
    return insert(get_hash(k), k, v, reinterpret_cast<CNode *>(root), 1);
}

bool Trie::insert(int hash, string k, int v, CNode *node, int level) {
    int path = get_part_by_level(hash, level);

    if (!node->bmp.is_set(path)) {
        node->addNode(create_s_node(k, v));
        return true;
    } else {
        node->replace_child_s_node_to_c_node(path);
        return insert(hash, k, v, static_cast<CNode *>(node->getNodeByPath(path)), level + 1);
    }
}
