#define protected public
#define private   public

#include "../include/trie.h"
#include "../include/utils.h"

#undef protected
#undef private

#include <gtest/gtest.h>

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

TEST(BITMAP, HAPPY_FLOW__SET_TRUE_AT_FALSE) {
    // arrange
    bitmap bmp = {0b1001};

    // act
    bmp.set(2);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b1101}.data);
}

TEST(BITMAP, HAPPY_FLOW__SET_TRUE_AT_TRUE) {
    // arrange
    bitmap bmp = {0b1001};

    // act
    bmp.set(3);

    // assert
    ASSERT_EQ(bmp.data, (bitmap) {0b1001}.data);
}

TEST(CNode, HAPPY_FLOW_GET__NODE_NOT_FOUND) {
    // arrange
    CNode node;
    node.bmp = {0b100001};

    // assert
    ASSERT_EQ(node.getSubNode(0b11111), nullptr);
}

TEST(CNode, HAPPY_FLOW_GET_NODE) {
    // arrange
    auto *first_node = new SNode(" ", 0, 0);
    auto *second_node = new SNode(" ", 0, 0);
    CNode node;
    node.bmp = {0b10001};
    node.array = {first_node, second_node};

    // act & assert
    ASSERT_EQ(node.getSubNode(0), first_node);
    ASSERT_EQ(node.getSubNode(4), second_node);
}

TEST(CNode, HAPPY_FLOW__GET_ARRAY_INDEX_BY_BMP) {
    // arrange
    CNode node;
    node.bmp = {0b100110101};

    // act & assert
    ASSERT_EQ(node.getArrayIndexByBmp(0), 0);
    ASSERT_EQ(node.getArrayIndexByBmp(2), 1);
    ASSERT_EQ(node.getArrayIndexByBmp(4), 2);
    ASSERT_EQ(node.getArrayIndexByBmp(5), 3);
    ASSERT_EQ(node.getArrayIndexByBmp(8), 4);
}


TEST(UTILS, HAPPY__FLOW_GET_PATH_OF_HASH_BY_LEVEL) {
    // arrange
    SNode node("abcdefg1", 0, generateHash("abcdefg1"));

    // act & assert
    ASSERT_EQ(node.getHashByLevel(0), 0b10111);
    ASSERT_EQ(node.getHashByLevel(1), 0b01110);
    ASSERT_EQ(node.getHashByLevel(3), 0b00000);
}


// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> i2 -> c2 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TWO_KEYS_WITH_SAME_PREFIX) {
    // arrange
    Trie trie;
    SNode *first = new SNode("", 1, 0b1010001);
    SNode *second = new SNode("", 2, 0b1110001);

    // act
    trie.insert(trie.root, first, 0);
    trie.insert(trie.root, second, 0);

    // assert
    INode *i2 = static_cast<INode *>(trie.root->main->getSubNode(0b10001));
    SNode *k1 = static_cast<SNode *>(i2->main->getSubNode(0b00010));
    SNode *k2 = static_cast<SNode *>(i2->main->getSubNode(0b00011));

    ASSERT_EQ(k1->value, 1);
    ASSERT_EQ(k2->value, 2);
}

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TWO_KEYS_WITH_DIFFERENT_PREFIXIES) {
    // arrange
    Trie trie;
    SNode *first = new SNode("", 1, 0b11111);
    SNode *second = new SNode("", 2, 0b00000);

    // act
    trie.insert(trie.root, first, 0);
    trie.insert(trie.root, second, 0);

    // assert
    SNode *k1 = static_cast<SNode *>(trie.root->main->getSubNode(0b11111));
    SNode *k2 = static_cast<SNode *>(trie.root->main->getSubNode(0b00000));

    ASSERT_EQ(k1->value, 1);
    ASSERT_EQ(k2->value, 2);
}

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> k1, (i2 -> c2 -> k2, k3)
TEST(TRIE, HAPPY_FLOW__INSERT_THREE_KEYS) {
    // arrange
    Trie trie;
    SNode *first = new SNode("", 1, 0b00000);
    SNode *second = new SNode("", 2, 0b00001);
    SNode *third = new SNode("", 3, 0b1111100001);

    // act
    trie.insert(trie.root, first, 0);
    trie.insert(trie.root, second, 0);
    trie.insert(trie.root, third, 0);

    // assert
    SNode *k1 = static_cast<SNode *>(trie.root->main->getSubNode(0b00000));
    INode *i2 = static_cast<INode *>(trie.root->main->getSubNode(0b00001));
    SNode *k2 = static_cast<SNode *>(i2->main->getSubNode(0b00000));
    SNode *k3 = static_cast<SNode *>(i2->main->getSubNode(0b11111));

    ASSERT_EQ(k1->value, 1);
    ASSERT_EQ(k2->value, 2);
    ASSERT_EQ(k3->value, 3);
}

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> i2 -> c2 -> i3 -> c3 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TWO_KEYS_WITH_SAME_BIGGER_PREFIX) {
    // arrange
    Trie trie;
    SNode *first = new SNode("", 1, 0b000001111100000);
    SNode *second = new SNode("", 2, 0b101011111100000);

    // act
    trie.insert(trie.root, first, 0);
    trie.insert(trie.root, second, 0);

    // assert
    INode *i2 = static_cast<INode *>(trie.root->main->getSubNode(0b000000));
    INode *i3 = static_cast<INode *>(i2->main->getSubNode(0b111111));
    SNode *k1 = static_cast<SNode *>(i3->main->getSubNode(0b00000));
    SNode *k2 = static_cast<SNode *>(i3->main->getSubNode(0b10101));

    ASSERT_EQ(k1->value, 1);
    ASSERT_EQ(k2->value, 2);
}

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> i2 -> c2 -> i3 -> c3 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TO_EMPTY_TRIE) {
    // arrange
    Trie trie;

    // act
    trie.insert("abc", 1);

    // assert
    ASSERT_NE(trie.root->main->getSubNode(8), nullptr);
    ASSERT_EQ(trie.root->main->getSubNode(0), nullptr);
}

// BEFORE:  i1 -> c1 -> k1
//  AFTER:  i1' -> c1' -> k1'
TEST(INODE, HAPPY_FLOW__SWAP_TO_COPY_WITH_REPLACED_CHILD_CREATE_NEW_COPY){
    // arrange
    CNode*c1 = new CNode();
    SNode* k1 = new SNode("", 1, 0);
    c1->insertChild(k1, 0b00000);
    INode* i1 = new INode(c1);

    // act
    i1->swapToCopyWithInsertedChild(new SNode("", 1, 1), 0b11111);

    // assert
    ASSERT_NE(i1->main, c1);
    ASSERT_NE(&i1->main->bmp, &c1->bmp);
    ASSERT_NE(&i1->main->array, &c1->array);
}

