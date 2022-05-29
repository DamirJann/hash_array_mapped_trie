#include "../include/utils.h"


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
