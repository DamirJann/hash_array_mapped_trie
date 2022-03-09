#include "../include/utils.h"

uint64_t generateHash(string k) {
    uint64_t hash = 0;
    for (size_t i = 0; i < k.size(); i++) {
        hash += k[i] * i;
    }
    return hash % 1000;
}

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level) {
    return (hash >> (level * HASH_PIECE_LEN)) % BRANCH_FACTOR;
}