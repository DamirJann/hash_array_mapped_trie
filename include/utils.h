#include <cstddef>
#include <cstdint>
#include <string>

#pragma once
#define HASH_PIECE_LEN 5
#define BRANCH_FACTOR 32

using namespace std;


uint64_t generateHash(string);
uint64_t generateHash(int);

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level);