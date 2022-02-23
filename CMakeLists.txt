cmake_minimum_required(VERSION 3.16)
project(hash_array_mapped_trie)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(google_tests/lib/googletest googletests)


# MAIN TARGET
add_executable(
        hash_array_mapped_trie
        src/trie.cpp main.cpp src/utils.cpp
)

# TEST TARGET
add_executable(
        run_tests
        test/test.cpp src/trie.cpp src/utils.cpp
)



target_link_libraries(
        run_tests PRIVATE
        gtest gtest_main
)

# COMPILATION FLAGS
target_compile_options(
        hash_array_mapped_trie PRIVATE
        -Wall -Wextra -pedantic -Werror
)

