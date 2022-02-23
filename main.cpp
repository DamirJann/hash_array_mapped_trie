#include <iostream>
#include <fstream>
#include "include/trie.h"
#include "include/utils.h"
//#pragma once

//using  namespace  std;
int main(){
    Trie trie;
    trie.insert("abc", 1);
    trie.insert("bcd", 2);
    trie.insert("ere", 4);
    trie.insert("gfdg", 5);
    trie.insert("vcb", 6);
    trie.insert("qwer", 7);
    trie.insert("bbbb", 8);
    trie.insert("sd", 8);
    trie.insert("sdvd", 8);
    trie.insert("sdsfdvd", 8);
    trie.insert("sdsfdsfdvd", 8);
    trie.insert("scvxdsfdsfdvd", 8);
    trie.insert("scddddsvxdsfdsfdvd", 8);
    trie.insert("scddddsvxdsqqqqfdsfdvd", 8);
    trie.insert("scdsdfdddsvxdsqqqqfdsfdvd", 8);
    trie.insert("1e23re", 8);
    trie.insert("sdf", 8);
    trie.insert("1234", 8);
    trie.insert("435", 8);
    trie.insert("676", 8);
    trie.insert("dfgvvv", 8);
    trie.insert("ew", 8);
    trie.insert("dfdcxxc vgvvv", 8);
    trie.insert("dfgvvvxcvxcvxcv", 8);

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}