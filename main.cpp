#include "include/trie.h"
#include "include/utils.h"
#include "include/visualize.h"

#include <random>
#include <pthread.h>
const char* fmt_thread_log = "LOG[%d] %s\n";


int main(){
    // arrange
    Trie<int, int> trie;
    int thread_count = 20;
    int iteration_count = 100000;

    vector<pthread_t> thread(thread_count);
    vector<vector<void *>> attr(thread_count);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(iteration_count)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *iteration_count = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*iteration_count); i < (*id + 1) * (*iteration_count); i++) {
                cout << "LOG[" <<  to_string(*id) <<  "]:" << " Inserted "  << i << endl;
                trie->insert(i, i);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

//    ofstream f = std::ofstream("graph.txt");
//    visualize(f, &trie);
//    system("dot -Tpng ./graph.txt -o graph.png");
//    f.close();

}