#include <cstddef>
#include <cstdint>
#include <string>
#include <cassert>
#include <iostream>

#pragma once
#define HASH_PIECE_LEN 5
#define BRANCH_FACTOR 32
#define MAX_LEVEL_COUNT (int (64 / 5))

using namespace std;

struct Bitmap {
    uint32_t data;

    bool isSet(uint8_t pos) const;

    void set(uint8_t pos);

    void unset(uint8_t pos);
};

uint64_t generateSimpleHash(uint64_t);

uint64_t generateSimpleHash(string);

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level);

std::pair<string, string>  findTwoKeyWithEqualHash();
