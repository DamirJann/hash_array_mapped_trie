cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(../lib/googletest googletests)

include_directories(../lib/benchmark/include ../lib/benchmark/build/include ../lib/libcds)
link_directories(../lib/benchmark/build/src ../lib/libcds/libcds-debug/bin)


# TEST TARGET
add_executable(
        unit_test unit_test.cpp ../src/utils.cpp
)

add_executable(
        benchmark_test benchmark_test.cpp ../src/utils.cpp
)


target_link_libraries(
        unit_test PRIVATE
        gtest gtest_main
)


target_link_libraries(
        benchmark_test
        cds
        libbenchmark.a
)
