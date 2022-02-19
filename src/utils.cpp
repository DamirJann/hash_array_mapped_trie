#include "../include/trie.h"

uint8_t get_path_by_level(uint64_t hash, uint8_t level) {
    return (hash >> (level * HASH_PIECE_MAX_LEN)) % BRANCH_FACTOR;
}

