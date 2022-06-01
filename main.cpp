#include "include/hamt.h"
#include "include/utils.h"
#include "include/visualize.h"

int main(){
    Hamt<string, int> trie;

//    for (int i = 0; i < 100; i++){
//        cout << trie.remove("k"+ to_string(i));
//        cout << " removed " << i << endl;
//    }



    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}