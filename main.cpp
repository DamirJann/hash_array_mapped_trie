#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"

#include <random>
#include <pthread.h>


int main(){
    // arrange
    Trie<int , uint64_t> trie;
    int thread_count = 1;
    int iteration_count = 100000;
//    for (int i = 0 ; i < 300; i++){
//        string key = "k"  + to_string(i);
//        trie.insert(key, i);
//    }
//    for (int i = 0 ; i < 300; i++){
//        string key = "k"  + to_string(i);
//        trie.remove(key);
//    }

    for (int i = 0; i < 32; i++){
        trie.insert(i, i);
    }
    for (int i = 32; i < 40; i++){
        trie.insert(i, i);
    }

    cout << trie.lookup(32).value;
//    vector<pthread_t> thread(thread_count);
//    vector<vector<void *>> attr(thread_count);
//    for (int i = 0; i < attr.size(); i++) {
//        attr[i] = {&trie, new int(i), new int(iteration_count)};
//    }
//
//    for (int i = 0; i < thread.size(); i++) {
//        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
//            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
//            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
//            int *iteration_count = (int *) (*static_cast<vector<void *> *>(args))[2];
//            for (int i = *id * (*iteration_count); i < (*id + 1) * (*iteration_count); i++) {
////                cout << "LOG[" <<  to_string(*id) <<  "]:" << " Inserted "  << i << endl;
//                trie->insert(i, i);
//            }
//            pthread_exit(nullptr);
//        }, &attr[i]);
//
//    }
//
//    for (unsigned long i: thread) {
//        pthread_join(i, nullptr);
//    }


    ofstream f = std::ofstream("graph.txt");
    visualize(f, &trie);
    system("dot -Tpng ./graph.txt -o graph.png");
    f.close();

}