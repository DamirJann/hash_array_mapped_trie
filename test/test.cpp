#define protected public
#define private   public

#include "../include/trie.h"

#undef protected
#undef private

#include "../include/utils.h"
#include <gtest/gtest.h>

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_BEGINNING_OF_BITMAP) {
    bitmap bmp = {0b1110010011};
    ASSERT_EQ(bmp.is_set(0), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_BEGINNING_OF_BITMAP) {
    bitmap bmp = {0b1110010010};
    ASSERT_EQ(bmp.is_set(0), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_MIDDLE_OF_BITMAP) {
    bitmap bmp = {0b1110010011};
    ASSERT_EQ(bmp.is_set(4), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_MIDDLE_OF_BITMAP) {
    bitmap bmp = {0b1110010010};
    ASSERT_EQ(bmp.is_set(3), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_END_OF_BITMAP) {
    bitmap bmp = {0x80000000};
    ASSERT_EQ(bmp.is_set(31), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_END_OF_BITMAP) {
    bitmap bmp = {0};
    ASSERT_EQ(bmp.is_set(31), false);
}

TEST(BITMAP, HAPPY_FLOW__SET_TRUE_AT_FALSE) {
    bitmap bmp = {0b1001};
    bmp.set(2);
    ASSERT_EQ(bmp.data, (bitmap) {0b1101}.data);
}

TEST(BITMAP, HAPPY_FLOW__SET_TRUE_AT_TRUE) {
    bitmap bmp = {0b1001};
    bmp.set(3);
    ASSERT_EQ(bmp.data, (bitmap) {0b1001}.data);
}

TEST(CNode, HAPPY_FLOW_GET_NODE_NOT_FOUND) {
    CNode node;
    node.bmp = {0b100001};
    ASSERT_EQ(node.getNode(0b11111), nullptr);
}

TEST(CNode, HAPPY_FLOW_GET_NODE) {
    auto *first_node = new SNode();
    auto *second_node = new SNode();

    CNode node;
    node.bmp = {0b10001};
    node.array = {first_node, second_node};

    ASSERT_EQ(node.getNode(0), first_node);
    ASSERT_EQ(node.getNode(4), second_node);
}

TEST(CNode, HAPPY_FLOW_GET_ARRAY_INDEX_BY_BMP) {
    CNode node;
    node.bmp = {0b100110101};

    ASSERT_EQ(node.get_array_index_by_bmp(0), 0);
    ASSERT_EQ(node.get_array_index_by_bmp(2), 1);
    ASSERT_EQ(node.get_array_index_by_bmp(4), 2);
    ASSERT_EQ(node.get_array_index_by_bmp(5), 3);
    ASSERT_EQ(node.get_array_index_by_bmp(8), 4);
}


TEST(UTILS, HAPPY_FLOW_GET_PATH_OF_HASH_BY_LEVEL) {
    uint64_t hash = 0b000000000010101110101100101010001;
    ASSERT_EQ(get_path_by_level(hash, 0), 0b10001);
    ASSERT_EQ(get_path_by_level(hash, 1), 0b01010);
    ASSERT_EQ(get_path_by_level(hash, 2), 0b10110);
    ASSERT_EQ(get_path_by_level(hash, 3), 0b01110);
    ASSERT_EQ(get_path_by_level(hash, 4), 0b00101);
    ASSERT_EQ(get_path_by_level(hash, 5), 0b00000);
}







