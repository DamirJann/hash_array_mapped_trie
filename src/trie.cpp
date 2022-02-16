#include "../include/trie.h"
#include <bit>
#include <bitset>

using namespace std;

int bitmap::get_array_index(int bitmap_index) {
    __popcount(
            ((1 << bitmap_index) - 1) && data
    );
}
