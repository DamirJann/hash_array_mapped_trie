#include <cstddef>
#include <cstdint>
#include <string>
#include <cassert>

#pragma once
#define HASH_PIECE_LEN 5
#define BRANCH_FACTOR 32
#define MAX_LEVEL_COUNT (int (64 / 5))

using namespace std;

uint64_t generateSimpleHash(string);
uint64_t generateSimpleHash(int);

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level);