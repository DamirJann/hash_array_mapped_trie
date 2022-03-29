#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"

#include <random>
#include <pthread.h>


int main(){
    // arrange
    Trie<string , uint64_t> trie;
    trie.insert("k2", 2);
    trie.insert("k20", 2);
    trie.insert("k23", 2);
    trie.remove("k23");

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}