#include <cstdint>
#include <string>
#include <vector>

using namespace std;

#define BRANCH_FACTOR 4

struct bitmap {
    int32_t data;
    bool is_set(int) const;
    void set(int);
} bitmap;

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
    struct bitmap bmp;
    vector<Node *> array;

    CNode() : Node() {
        type = C_NODE;
        bmp = {0};
    }

    CNode(SNode* node): CNode(){
        addNode(node);
    }

    // TODO optimize it
    void addNode(SNode*);
    void replace_child_s_node_to_c_node(int);
    Node* getNodeByPath(int);
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

    static bool insert(int hash, string k, int v, CNode *node, int level);
};

struct value_result {
    int value;
    bool is_found;
};

value_result new_value_result(int value) {
    return (value_result) {.value = value, .is_found = true};
}

const value_result value_not_found{
        .is_found = true
};