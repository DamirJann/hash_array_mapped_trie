cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

add_subdirectory(../lib/googletest googletests)

include_directories(../../benchmark/include ../../benchmark/build/include ../../libcds)
link_directories(../../benchmark/build/src ../../libcds/libcds-debug/bin ../lib/googletest)


# TEST TARGET
add_executable(
        unit_test unit_test.cpp ../src/utils.cpp
)

add_executable(
        benchmark_test benchmark_test.cpp ../src/utils.cpp
)


target_link_libraries(
        unit_test PRIVATE
        gtest gtest_main.a
)


target_link_libraries(
        benchmark_test
        cds
        libbenchmark.a
)
