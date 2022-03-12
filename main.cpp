#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"

int main(){
    Trie<string, int> trie;

    for (int i = 0; i < 200; i++){
        fprintf(stdout, "for k%d hash is %lu\n", i, generateSimpleHash("k"+ to_string(i)));
        trie.insert("k"+ to_string(i), i);
    }
    trie.remove("k300");


    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}