#include <cstddef>
#include <cstdint>
#include <string>
#include "../include/trie.h"


using namespace std;

uint64_t generateHash(string);
uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level);