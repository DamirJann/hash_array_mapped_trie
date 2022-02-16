#include <cstdint>
#include <string>

using namespace std;

#define BRANCH_FACTOR 2

class Node {

};

class Trie {
private:
    Node root;
public:
    bool lookup(string k);
    bool remove(string k);
    bool insert(string k, int value);
};


typedef struct {
    int32_t data;
    int get_array_index(int bitmap_index);
} bitmap;

class CNode : public Node{
public:
    bitmap bmp;
    Node array[BRANCH_FACTOR];
};

class SNode : public Node{
public:
    string key;
    int value;
};

struct value_result{
    int value;
    bool is_found;
};

value_result new_value_result(int value){
    return (value_result){.value = value, .is_found = true};
}

const value_result value_not_found{
    .is_found = true
};