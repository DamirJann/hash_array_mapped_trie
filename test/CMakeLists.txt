cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(../lib/googletest googletests)

# TEST TARGET
add_executable(
        run_tests test.cpp ../src/utils.cpp
)

target_link_libraries(
        run_tests PRIVATE
        gtest gtest_main
)

