#include <benchmark/benchmark.h>
#include "../include/hamt.h"
#include <unordered_map>
#include <armadillo>
#include "cds/init.h"
#include "cds/gc/hp.h"
#include "cds/container/michael_kvlist_hp.h"

int threadCount = stoi(std::getenv("THREAD_COUNT"));
int insertCount = stoi(std::getenv("INSERT_COUNT"));

// -----------------------------------------------------------------
// -----------------------------------------------------------------

static void Hamt_Insert(benchmark::State &state) {
    for (auto _: state) {
        Hamt<int, int> hamt;
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&hamt, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    hamt->insert(i, i);
                }
                pthread_exit(nullptr);
            }, &attr[i]);

        }

        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
}

BENCHMARK(Hamt_Insert)->Repetitions(1)->Iterations(1);

// -----------------------------------------------------------------
// -----------------------------------------------------------------

static void Hamt_Lookup(benchmark::State &state) {
    Hamt<int, int> hamt;
    for (int i = 0; i < threadCount * insertCount; i++) {
        hamt.insert(i, i);
    }
    for (auto _: state) {
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&hamt, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    assert(hamt->lookup(i).value == i);
                }
                pthread_exit(nullptr);
            }, &attr[i]);
        }
        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
    cds::Terminate();
}

BENCHMARK(Hamt_Lookup)->Repetitions(1)->Iterations(1);


// -----------------------------------------------------------------
// -----------------------------------------------------------------

static void Hamt_Remove(benchmark::State &state) {
    Hamt<int, int> hamt;
    for (int i = 0; i < threadCount * insertCount; i++) {
        hamt.insert(i, i);
    }
    for (auto _: state) {
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&hamt, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    assert(hamt->remove(i) == true);
                }
                pthread_exit(nullptr);
            }, &attr[i]);
        }
        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
    cds::Terminate();
}

BENCHMARK(Hamt_Remove)->Repetitions(1)->Iterations(1);


BENCHMARK_MAIN();

