#include "include/hamt.h"
#include "include/utils.h"
#include "include/visualize.h"
#include <assert.h>

#include <random>
#include <pthread.h>


int main(){
    Hamt<int, int> hamt;
    int count = 34;

    // act & assert
    for (int i = 0; i < count; i++) {
        hamt.insert(i, i);
    }
//
    for (int i = 0; i < count; i++) {
        assert(hamt.lookup(i) == hamt.createSuccessfulLookupResult(i));
    }

//    hamt.remove(0);
//    hamt.remove(1);
    for (int i = 0; i < count; i++) {
//        Hamt<int,int>::RemoveResult r = hamt.remove(i);
//        assert(r == hamt.createSuccessfulRemoveResult(i));
    }
//
//    for (int i = 0; i < count; i++) {
//        ASSERT_EQ(hamt.lookup(i), LOOKUP_NOT_FOUND);
//    }

    ofstream f = std::ofstream("graph.txt");
    visualize(f, &hamt);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}