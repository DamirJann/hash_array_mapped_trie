//#include <benchmark/benchmark.h>
//#include "../include/hamt.h"
//#include <unordered_map>
//
//
//int threadCount = 4;
//int insertCountByThread = 5'000'000;
//
//static void Hamt_Insert(benchmark::State& state) {
//    for (auto _ : state){
//        // arrange
//        Hamt<int, int> hamt;
//
//        vector<pthread_t> thread(threadCount);
//        vector<vector<void *>> attr(threadCount);
//        for (int i = 0; i < attr.size(); i++) {
//            attr[i] = {&hamt, new int(i), new int(insertCountByThread)};
//        }
//
//        for (int i = 0; i < thread.size(); i++) {
//            pthread_create(&thread[i], nullptr, [](void *args) -> void * {
//                auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
//                int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
//                int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
//
//                for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
//                    hamt->insert(i, i);
//                }
//
//                pthread_exit(nullptr);
//            }, &attr[i]);
//
//        }
//
//        for (unsigned long i: thread) {
//            pthread_join(i, nullptr);
//        }
//    }
//}
//BENCHMARK(Hamt_Insert)->Repetitions(1)->Iterations(1);
//
//
//
//static void Set_Insert(benchmark::State& state) {
//    for (auto _ : state){
//        set<int> set;
//        for (int i = 0; i < threadCount * insertCountByThread; i++){
//            set.insert(i);
//        }
//    }
//}
//BENCHMARK(Set_Insert)->Repetitions(1)->Iterations(1);
//
//
//static void Map_Insert(benchmark::State& state) {
//    for (auto _ : state){
//        unordered_map<int, int> map;
//
//        for (int i = 0; i < threadCount * insertCountByThread; i++){
//            map.insert({i, i});
//        }
//    }
//}
//BENCHMARK(Map_Insert)->Repetitions(1)->Iterations(1);
//
//
//
//BENCHMARK_MAIN();