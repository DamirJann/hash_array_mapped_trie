#include <benchmark/benchmark.h>
#include <armadillo>
#include "include/hamt.h"


// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
    for (auto _ : state){
        unsigned int microseconds = 10000000;
        usleep(microseconds);
    }

}
BENCHMARK(BM_StringCopy)->Repetitions(5)->Iterations(1);

BENCHMARK_MAIN();