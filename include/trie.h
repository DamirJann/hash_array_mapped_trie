#include <cstdint>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

#define HASH_PIECE_MAX_LEN 5
#define BRANCH_FACTOR 32

struct bitmap {
    uint32_t data;

    bool is_set(uint8_t) const;

    void set(uint8_t);
};

enum NodeType {
    C_NODE,
    S_NODE,
};

class Node {
public:
    NodeType type;
};

class SNode : public Node {
public:
    string key;
    int value;

    SNode() : Node() {
        this->type = S_NODE;
    }
};

class CNode : public Node {
public:
    friend class TestCNode;
    CNode() : Node() {
        type = C_NODE;
        bmp = {0};
    }
    CNode(SNode *node) : CNode() {
        addNode(node);
    }

    void addNode(SNode *);
    void replace_child_s_node_to_c_node(int);
    Node *getNode(uint8_t);
private:
    bitmap bmp;
    vector<Node *> array;
    uint8_t get_array_index_by_bmp(uint8_t) const;
};

class Trie {
private:
    Node *root;
public:
    bool lookup(string k);

    bool remove(string k);

    bool insert(string k, int v);

private:
    bool lookup(int hash);

    bool remove(int hash);

    static bool insert(uint64_t hash, string k, int v, CNode *node, uint8_t level);
};

struct value_result {
    int value;
    bool is_found;
};

inline value_result new_value_result(int value) {
    return (value_result) {.value = value, .is_found = true};
}

const value_result value_not_found{
        .is_found = true
};