#include <cstdint>
#include <string>
#include <vector>

using namespace std;

#define BRANCH_FACTOR 2

enum NodeType {
    C_NODE,
    S_NODE,
};

class Node {
public:
    NodeType type;
};

class Trie {
private:
    Node *root;
public:
    bool lookup(string k);

    bool remove(string k);

    bool insert(string k, int value);

private:
    bool lookup(int hash);

    bool remove(int hash);

    static bool insert(int hash, int value, Node *currNode, int level);
};


typedef struct {
    int32_t data;

    int get_array_index(int bitmap_index);
} bitmap;

class CNode : public Node {
public:
    bitmap bmp;
    vector<Node *> array;
    CNode(): Node(){
        type = C_NODE;
        bmp = {0};
    }
};

class SNode : public Node {
public:
    string key;
    int value
    SNode() : Node(){
        this->type = S_NODE;
    }
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