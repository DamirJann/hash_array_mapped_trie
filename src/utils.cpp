#include "../include/utils.h"



uint64_t generateSimpleHash(int key){
    return key;
}

uint64_t generateSimpleHash(string key) {
    uint64_t hash = 0;
    for (size_t i = 0; i < key.size(); i++) {
        hash += key[i] * i;
    }
    return hash;
}

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level) {
    return (hash >> (level * HASH_PIECE_LEN)) % BRANCH_FACTOR;
}