#include <cstddef>
#include <cstdint>
#include <string>
#include <cassert>

#pragma once
#define HASH_PIECE_LEN 5
#define BRANCH_FACTOR 32

using namespace std;


//string fmt_assert_failed = "Assert failed: %s != %s";

template<class A, class B>
void ASSERT_EQUAL(A got,B exp){
    if (got != exp){

    }
}


uint64_t generateSimpleHash(string);
uint64_t generateSimpleHash(int);

uint8_t extractHashPartByLevel(uint64_t hash, uint8_t level);