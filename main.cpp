#include <iostream>
#include <fstream>
#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"

int main(){
    Trie trie;

    trie.insert("k1", 1);
    trie.insert("k1", 2);
    trie.insert("k1", 3);
    trie.insert("k1", 4);
    trie.insert("k1", 5);
    trie.insert("k1", 6);
    trie.insert("k1", 7);
    trie.insert("k1", 8);
    trie.insert("k2", 1);
    trie.insert("k3", 2);
    trie.insert("k4", 1);
    trie.insert("k5", 5);
    trie.insert("k6", 7);
    trie.insert("k7", 2);
    trie.insert("k8", 9);
    trie.insert("k9", 7);
    trie.insert("k10", 8);
    trie.insert("k12", 12);
    trie.insert("k13", 13);
    trie.insert("k14", 14);
    trie.insert("k15", 15);
    trie.insert("k16", 16);
    trie.insert("k17", 17);
    trie.insert("k18", 18);
    trie.insert("k19", 19);
    trie.insert("k20", 20);
    trie.insert("k21", 21);
    trie.insert("k22", 22);
    trie.insert("k23", 23);
    trie.insert("k24", 24);
    trie.insert("k25", 25);
    trie.insert("k26", 26);
    trie.insert("k27", 27);
    trie.insert("k28", 28);
    trie.insert("k29", 29);
    trie.insert("k31", 31);
    trie.insert("k32", 32);
    trie.insert("k33", 33);
    trie.insert("k34", 34);
    trie.insert("k35", 35);
    trie.insert("k36", 36);
    trie.insert("k37", 37);
    trie.insert("k38", 38);
    trie.insert("k39", 39);
    trie.insert("k40", 40);
//
//    trie.remove("k1");
//    trie.remove("k8");

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}