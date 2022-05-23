#include "../include/utils.h"


uint64_t generateSimpleHash(uint64_t key){
    return key;
}

uint64_t generateSimpleHash(string key) {
    uint64_t hash = 0;
    for (size_t i = 0; i < key.size(); i++) {
        hash += key[i] * i;
    }

    return hash % 1321;
}

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level) {
    return (hash >> (level * HASH_PIECE_LEN)) & ((1 << HASH_PIECE_LEN) - 1);
}

bool Bitmap::isSet(uint8_t pos) const {
    return ((data >> pos) & 0b1) == 0b1;
}

void Bitmap::set(uint8_t pos) {
    data = data | (1 << pos);
}

void Bitmap::unset(uint8_t pos) {
    data = data & (~(1 << pos));
}

std::pair<string, string>  findTwoKeyWithEqualHash(){
    std::pair<string, string> res;
    res.first = "k0";
    res.second = "k1";
    int i = 2;
    while (generateSimpleHash(res.first) != generateSimpleHash(res.second)){
        res.second = "k" + to_string(i);
        i++;
    }
    return res;
};