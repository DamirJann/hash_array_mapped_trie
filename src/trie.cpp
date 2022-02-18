#include "../include/trie.h"
#include <bit>
#include <bitset>
#include <utility>

using namespace std;

int bitmap::get_array_index(int bitmap_index) {
    return __popcount(
            ((1 << bitmap_index) - 1) && data
    );
}

int get_hash(string k) {
    int hash = 0;
    for (int i = 0; i < k.size(); i++) {
        hash += k[i] * i;
    }
    return  hash % 10;
}

class SNode* create_s_node() {
     auto* node = new SNode();
     return node;
}

class CNode* create_c_node() {
    auto* node = new CNode();
    return node;
}


bool Trie::insert(string k, int value) {
    return insert(get_hash(k), value, root, 1);
}

bool Trie::insert(int hash, int value, Node *curr_node, int level) {
    int path = hash % 4;
    i
    return true;
}
