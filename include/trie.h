#include <cstdint>
#include <string>
#include <vector>

using namespace std;

#define HASH_PIECE_MAX_LEN 5
#define BRANCH_FACTOR 32

struct bitmap {
    uint32_t data;

    bool isSet(uint8_t pos) const;

    void set(uint8_t);
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

class SNode : public Node {
public:
    string key;
    int value;

    uint8_t getHashByLevel(uint8_t);

    SNode(string k, int v, uint64_t hash) : Node(S_NODE) {
        this->key = k;
        this->value = v;
        this->hash = hash;
    }

private:
    uint64_t hash;
};

class CNode : public Node {
public:
    friend class TestCNode;

    CNode() : Node(C_NODE) {
        bmp = {0};
    }
    Node *getSubNode(uint8_t);

    void insertChild(Node *newChild, uint8_t path);
    void replaceChild(Node *newChild, uint8_t path);

private:
    bitmap bmp;
    vector<Node *> array;
    uint8_t getArrayIndexByBmp(uint8_t) const;
};

class INode : public Node{
public:
    INode(CNode* main): Node(I_NODE){
        this->main = main;
    }

    bool swapToCopyWithReplacedChild(INode *newChild, uint8_t path);
    bool swapToCopyWithInsertedChild(Node *, uint8_t);
    CNode* main;
};

class Trie {
private:
    INode *root;
public:
    Trie(){
        root = new INode(new CNode());
    }

    bool lookup(string k);

    bool remove(string k);

    bool insert(string k, int v);

private:
    bool lookup(int hash);

    bool remove(int hash);

    static bool insert(INode *, SNode *, uint8_t );
};
