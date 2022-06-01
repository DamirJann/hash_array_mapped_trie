
#pragma once
#define protected public
#define private   public

#include "../include/hamt.h"

#undef protected
#undef private

#include <gtest/gtest.h>
#include <random>


TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_BEGINNING_OF_BITMAP) {
    // arrange
    bitmap bmp = {0b1110010011};
    ASSERT_EQ(bmp.isSet(0), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_BEGINNING_OF_BITMAP) {
    // arrange
    bitmap bmp = {0b1110010010};

    // act & assert
    ASSERT_EQ(bmp.isSet(0), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_MIDDLE_OF_BITMAP) {
    // arrange
    bitmap bmp = {0b1110010011};

    // act & assert
    ASSERT_EQ(bmp.isSet(4), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_MIDDLE_OF_BITMAP) {
    // arrange
    bitmap bmp = {0b1110010010};

    // act & assert
    ASSERT_EQ(bmp.isSet(3), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_END_OF_BITMAP) {
    // arrange
    bitmap bmp = {0x80000000};

    // act & assert
    ASSERT_EQ(bmp.isSet(31), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_END_OF_BITMAP) {
    // arrange
    bitmap bmp = {0};

    // act & assert
    ASSERT_EQ(bmp.isSet(31), false);
}

TEST(BITMAP, HAPPY_FLOW__SET_AT_FALSE) {
    // arrange
    bitmap bmp = {0b1001};

    // act
    bmp.set(2);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b1101}.data);
}

TEST(BITMAP, HAPPY_FLOW__SET_AT_TRUE) {
    // arrange
    bitmap bmp = {0b1001};

    // act
    bmp.set(3);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b1001}.data);
}

TEST(BITMAP, HAPPY_FLOW__UNSET_AT_TRUE) {
    // arrange
    bitmap bmp = {0b10101};

    // act
    bmp.unset(2);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b10001}.data);
}

TEST(BITMAP, HAPPY_FLOW__UNSET_AT_FALSE) {
    // arrange
    bitmap bmp = {0b10101};

    // act
    bmp.unset(3);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b10101}.data);
}

TEST(BITMAP, HAPPY_FLOW__EXTRACT_HASH_PART) {
    // arrange
    uint64_t hash = 0b11111'00000'01001;

    // act & assert
    ASSERT_EQ(extractHashPartByLevel(hash, 0), 0b01001);
    ASSERT_EQ(extractHashPartByLevel(hash, 1), 0b00000);
    ASSERT_EQ(extractHashPartByLevel(hash, 2), 0b11111);
}

TEST(CNode, HAPPY_FLOW__GET_COPY) {
    // arrange

    auto *c1 = new CNode<int, int>;
    auto *k = new SNode<int, int>(4, 5, 4);
    c1->insertChild(k, 4);

    // act
    auto *c2 = new CNode<int, int>(*c1);

    // assert
    ASSERT_NE(c1, c2);
    ASSERT_EQ(c1->array[0], c2->array[0]);
    ASSERT_EQ(c1->bmp.data, c2->bmp.data);
}

TEST(CNode, HAPPY_FLOW_GET__NODE_LOOKUP_NOT_FOUND) {
    // arrange
    CNode<string, int> node;
    node.bmp = {0b100001};

    // assert
    ASSERT_EQ(node.getSubNode(0b11111), nullptr);
}

TEST(TRIE, HAPPY_FLOW__LOOKUP) {
    // arrange
    Hamt<string, int> hamt;

    hamt.insert("k1", 1);
    hamt.insert("k2", 2);
    hamt.insert("k3", 3);
    hamt.insert("k4", 4);
    hamt.insert("k5", 5);
    hamt.insert("k6", 6);
    hamt.insert("k7", 7);
    hamt.insert("k8", 8);
    hamt.insert("k9", 9);
    hamt.insert("k10", 10);
    hamt.insert("k11", 11);
    hamt.insert("k12", 12);
    hamt.insert("k13", 13);
    hamt.insert("k14", 14);
    hamt.insert("k15", 15);
    hamt.insert("k16", 16);
    hamt.insert("k17", 17);
    hamt.insert("k18", 18);
    hamt.insert("k19", 19);
    hamt.insert("k20", 20);
    hamt.insert("k21", 21);
    hamt.insert("k22", 22);
    hamt.insert("k23", 23);
    hamt.insert("k24", 24);
    hamt.insert("k25", 25);

    // act & assert
    ASSERT_EQ(hamt.lookup("k1").value, 1);
    ASSERT_EQ(hamt.lookup("k2").value, 2);
    ASSERT_EQ(hamt.lookup("k3").value, 3);
    ASSERT_EQ(hamt.lookup("k4").value, 4);
    ASSERT_EQ(hamt.lookup("k5").value, 5);
    ASSERT_EQ(hamt.lookup("k6").value, 6);
    ASSERT_EQ(hamt.lookup("k7").value, 7);
    ASSERT_EQ(hamt.lookup("k8").value, 8);
    ASSERT_EQ(hamt.lookup("k9").value, 9);
    ASSERT_EQ(hamt.lookup("k10").value, 10);
    ASSERT_EQ(hamt.lookup("k11").value, 11);
    ASSERT_EQ(hamt.lookup("k12").value, 12);
    ASSERT_EQ(hamt.lookup("k13").value, 13);
    ASSERT_EQ(hamt.lookup("k14").value, 14);
    ASSERT_EQ(hamt.lookup("k15").value, 15);
    ASSERT_EQ(hamt.lookup("k16").value, 16);
    ASSERT_EQ(hamt.lookup("k17").value, 17);
    ASSERT_EQ(hamt.lookup("k18").value, 18);
    ASSERT_EQ(hamt.lookup("k19").value, 19);
    ASSERT_EQ(hamt.lookup("k20").value, 20);
    ASSERT_EQ(hamt.lookup("k21").value, 21);
    ASSERT_EQ(hamt.lookup("k22").value, 22);
    ASSERT_EQ(hamt.lookup("k23").value, 23);
    ASSERT_EQ(hamt.lookup("k24").value, 24);
    ASSERT_EQ(hamt.lookup("k25").value, 25);

    ASSERT_EQ(hamt.lookup("k223").isFound, false);
    ASSERT_EQ(hamt.lookup("k224").isFound, false);
    ASSERT_EQ(hamt.lookup("k225").isFound, false);
    ASSERT_EQ(hamt.lookup("k226").isFound, false);
    ASSERT_EQ(hamt.lookup("k227").isFound, false);
    ASSERT_EQ(hamt.lookup("k228").isFound, false);
    ASSERT_EQ(hamt.lookup("k229").isFound, false);
    ASSERT_EQ(hamt.lookup("k2210").isFound, false);
    ASSERT_EQ(hamt.lookup("k2211").isFound, false);
    ASSERT_EQ(hamt.lookup("k2212").isFound, false);
    ASSERT_EQ(hamt.lookup("k2213").isFound, false);
    ASSERT_EQ(hamt.lookup("k2214").isFound, false);
    ASSERT_EQ(hamt.lookup("k2215").isFound, false);
    ASSERT_EQ(hamt.lookup("k2216").isFound, false);
    ASSERT_EQ(hamt.lookup("k2217").isFound, false);
    ASSERT_EQ(hamt.lookup("k2218").isFound, false);
    ASSERT_EQ(hamt.lookup("k2229").isFound, false);
    ASSERT_EQ(hamt.lookup("k2220").isFound, false);
    ASSERT_EQ(hamt.lookup("k2222").isFound, false);
    ASSERT_EQ(hamt.lookup("k2223").isFound, false);
    ASSERT_EQ(hamt.lookup("k2224").isFound, false);
    ASSERT_EQ(hamt.lookup("k2225").isFound, false);
    ASSERT_EQ(hamt.lookup("k2226").isFound, false);
    ASSERT_EQ(hamt.lookup("k227").isFound, false);
    ASSERT_EQ(hamt.lookup("k2228").isFound, false);
    ASSERT_EQ(hamt.lookup("k22d29").isFound, false);

}

TEST(TRIE, HAPPY_FLOW__REMOVE_ALL_KEYS) {
    // arrange
    Hamt<string, int> hamt;
    hamt.insert("k1", 1);
    hamt.insert("k2", 2);
    hamt.insert("k3", 3);
    hamt.insert("k30", 30);
    hamt.insert("k4", 4);

    // act
    ASSERT_EQ(hamt.remove("k1"), true);
    ASSERT_EQ(hamt.remove("k2"), true);
    ASSERT_EQ(hamt.remove("k30"),true);
    ASSERT_EQ(hamt.remove("k3"), true);
    ASSERT_EQ(hamt.remove("k4"), true);
    ASSERT_EQ(hamt.remove("k5"), false);
    ASSERT_EQ(hamt.remove("k7"), false);

    // assert
    ASSERT_EQ(hamt.lookup("k1").isFound, false);
    ASSERT_EQ(hamt.lookup("k2").isFound, false);
    ASSERT_EQ(hamt.lookup("k3").isFound, false);
    ASSERT_EQ(hamt.lookup("k30").isFound, false);
    ASSERT_EQ(hamt.lookup("k4").isFound, false);

}

TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_KEYS_BY_ONE_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int count = 100000;

    // act & assert
    for (int i = 0; i < count; i++) {
        hamt.insert(i, i);
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(hamt.lookup(i).isFound, true);
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(hamt.remove(i), true);
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(hamt.lookup(i).isFound, false);
    }

}
