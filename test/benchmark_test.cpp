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

static void Set_Insert(benchmark::State &state) {
    for (auto _: state) {
        set<int> set;
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        mutex mutex;
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&set, new int(i), new int(insertCount / threadCount), &mutex};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *set = (std::set<int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
                auto *mutex = (std::mutex *) (*static_cast<vector<void *> *>(args))[3];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    mutex->lock();
                    set->insert(i);
                    mutex->unlock();
                }
                pthread_exit(nullptr);
            }, &attr[i]);

        }

        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
}

static void MichaelKVList_Insert(benchmark::State &state) {
    cds::Initialize();
    cds::gc::HP hpGC;
    cds::threading::Manager::attachThread();
    for (auto _: state) {
        cds::container::MichaelKVList<cds::gc::HP, int, int> michaelKvList;
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&michaelKvList, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                cds::threading::Manager::attachThread();
                auto *michaelKvList = (cds::container::MichaelKVList<cds::gc::HP, int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    michaelKvList->insert(i, i);
                }
                cds::threading::Manager::detachThread();
                pthread_exit(nullptr);
            }, &attr[i]);
        }
        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
    cds::Terminate();
}


BENCHMARK(Hamt_Insert)->Repetitions(1)->Iterations(1);
BENCHMARK(Set_Insert)->Repetitions(1)->Iterations(1);
BENCHMARK(MichaelKVList_Insert)->Repetitions(1)->Iterations(1);

// -----------------------------------------------------------------
// -----------------------------------------------------------------

static void Hamt_Lookup(benchmark::State &state) {
    Hamt<int, int> hamt;
    for (int i = 0; i < insertCount; i++) {
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

static void Set_Lookup(benchmark::State &state) {
    set<int> set;
    for (int i = 0; i < insertCount; i++) {
        set.insert(i);
        cout << "inserting " << i << endl;
    }
    cout << "finished";
    for (auto _: state) {
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        mutex mutex;
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&set, new int(i), new int(insertCount / threadCount), &mutex};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *s = (std::set<int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
                auto *mutex = (std::mutex *) (*static_cast<vector<void *> *>(args))[3];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    mutex->lock();
                    s->find(i);
                    mutex->unlock();
                }
                pthread_exit(nullptr);
            }, &attr[i]);

        }

        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
}

static void MichaelKVList_Lookup(benchmark::State &state) {
    cds::Initialize();
    cds::gc::HP hpGC;
    cds::threading::Manager::attachThread();
    cds::container::MichaelKVList<cds::gc::HP, int, int> michaelKvList;

    for (int i = 0; i < insertCount; i++) {
        michaelKvList.insert(i, i);
    }

    for (auto _: state) {
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&michaelKvList, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                cds::threading::Manager::attachThread();
                auto *michaelKvList = (cds::container::MichaelKVList<cds::gc::HP, int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    assert(michaelKvList->contains(i) == true);
                }
                cds::threading::Manager::detachThread();
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
BENCHMARK(Set_Lookup)->Repetitions(1)->Iterations(1);
BENCHMARK(MichaelKVList_Lookup)->Repetitions(1)->Iterations(1);


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
                    assert(hamt->remove(i).value == i);
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

static void Set_Remove(benchmark::State &state) {
    for (auto _: state) {
        set<int> set;
        vector<pthread_t> thread(threadCount);
        mutex mutex;
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&set, new int(i), new int(insertCount / threadCount), &mutex};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                auto *set = (std::set<int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
                auto *mutex = (std::mutex *) (*static_cast<vector<void *> *>(args))[3];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    mutex->lock();
                    set->erase(i);
                    mutex->unlock();
                }
                pthread_exit(nullptr);
            }, &attr[i]);

        }

        for (unsigned long i: thread) {
            pthread_join(i, nullptr);
        }
    }
}

static void MichaelKVList_Remove(benchmark::State &state) {
    cds::Initialize();
    cds::gc::HP hpGC;
    cds::threading::Manager::attachThread();
    cds::container::MichaelKVList<cds::gc::HP, int, int> michaelKvList;

    for (int i = 0; i < insertCount; i++) {
        michaelKvList.insert(i, i);
    }

    for (auto _: state) {
        vector<pthread_t> thread(threadCount);
        vector<vector<void *>> attr(threadCount);
        for (int i = 0; i < attr.size(); i++) {
            attr[i] = {&michaelKvList, new int(i), new int(insertCount / threadCount)};
        }
        for (int i = 0; i < thread.size(); i++) {
            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
                cds::threading::Manager::attachThread();
                auto *michaelKvList = (cds::container::MichaelKVList<cds::gc::HP, int, int> *) (*static_cast<vector<void *> *>(args))[0];
                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                    assert(michaelKvList->erase(i) == true);
                }
                cds::threading::Manager::detachThread();
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
BENCHMARK(Set_Remove)->Repetitions(1)->Iterations(1);
BENCHMARK(MichaelKVList_Remove)->Repetitions(1)->Iterations(1);


BENCHMARK_MAIN();

