#include <iostream>
#include <fstream>
#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"
//#pragma once

//using  namespace  std;
int main(){
    Trie trie;

    trie.insert("k1", 1);
    trie.insert("k2", 1);
    trie.insert("k3", 2);
    trie.insert("k4", 1);
    trie.insert("k5", 5);
    trie.insert("k6", 7);
    trie.insert("k7", 2);
    trie.insert("k8", 9);
    trie.insert("k9", 7);
    trie.insert("k10", 8);
    trie.insert("k12", 0);
    trie.insert("k13", 0);
    trie.insert("k14", 0);
    trie.insert("k15", 0);
    trie.insert("k16", 0);
    trie.insert("k17", 0);
    trie.insert("k18", 0);
    trie.insert("k19", 0);
    trie.insert("k20", 0);
    trie.insert("k21", 0);
    trie.insert("k22", 0);
    trie.insert("k23", 0);
    trie.insert("k24", 0);
    trie.insert("k25", 0);
    trie.insert("k26", 0);
    trie.insert("k27", 0);
    trie.insert("k28", 0);
    trie.insert("k29", 0);
    trie.insert("k30", 0);
    trie.insert("k370", 0);
    trie.insert("k371", 0);
    trie.insert("k372", 0);
    trie.insert("k373", 0);
    trie.insert("k374", 0);
    trie.insert("k375", 0);
    trie.insert("k376", 0);

    trie.remove("k1");
    trie.remove("k8");

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}