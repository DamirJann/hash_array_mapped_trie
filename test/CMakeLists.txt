cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

#add_subdirectory(../lib/googletest-release/lib googletests)

include_directories(
        ../lib/benchmark-release/include
        ../lib/benchmark-release/build/include
        ../lib/libcds-release/libcds
        ../lib/googletest-release/googletest/include
)
link_directories(
        ../lib/benchmark-release/build/src
        ../lib/libcds-release/bin
        ../lib/googletest-release/lib
)


# TEST TARGET
add_executable(
        unit_test unit_test.cpp ../src/utils.cpp
)

add_executable(
        benchmark_test benchmark_test.cpp ../src/utils.cpp
)


target_link_libraries(
        unit_test gtest gtest_main
)


target_link_libraries(
        benchmark_test
        cds
        libbenchmark.a
)
