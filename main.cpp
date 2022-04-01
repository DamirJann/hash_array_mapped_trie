#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"
#include <assert.h>

#include <random>
#include <pthread.h>


int main(){
    Trie<int, int> trie;
    int count = 34;

    // act & assert
    for (int i = 0; i < count; i++) {
        trie.insert(i, i);
    }
//
    for (int i = 0; i < count; i++) {
        assert(trie.lookup(i) == createSuccessfulLookupResult(i));
    }

//    trie.remove(0);
//    trie.remove(1);
    for (int i = 0; i < count; i++) {
        RemoveResult r = trie.remove(i);
        cout << r.status << ' ' << r.value  << endl;
        assert(r == createSuccessfulRemoveResult(i));
    }
//
//    for (int i = 0; i < count; i++) {
//        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
//    }

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}