#include "../include/utils.h"

uint64_t generateSimpleHash(int key){
    return std::hash<int>{}(key);
}

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level) {
    return (hash >> (level * HASH_PIECE_LEN)) % BRANCH_FACTOR;
}