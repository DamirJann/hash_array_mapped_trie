
#pragma once
#define protected public
#define private   public

#include "../include/hamt.h"

#undef protected
#undef private

#include <gtest/gtest.h>
#include <random>

int averageIterationCount = 60'000;

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_BEGINNING_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0b1110010011};
    ASSERT_EQ(bmp.isSet(0), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_BEGINNING_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0b1110010010};

    // act & assert
    ASSERT_EQ(bmp.isSet(0), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_MIDDLE_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0b1110010011};

    // act & assert
    ASSERT_EQ(bmp.isSet(4), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_MIDDLE_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0b1110010010};

    // act & assert
    ASSERT_EQ(bmp.isSet(3), false);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_TRUE_AT_THE_END_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0x80000000};

    // act & assert
    ASSERT_EQ(bmp.isSet(31), true);
}

TEST(BITMAP, HAPPY_FLOW__IS_SET_FALSE_AT_THE_END_OF_BITMAP) {
    // arrange
    Bitmap bmp = {0};

    // act & assert
    ASSERT_EQ(bmp.isSet(31), false);
}

TEST(BITMAP, HAPPY_FLOW__SET_AT_FALSE) {
    // arrange
    Bitmap bmp = {0b1001};

    // act
    bmp.set(2);

    // assert
    ASSERT_EQ(bmp.data, (Bitmap) {0b1101}.data);
}

TEST(BITMAP, HAPPY_FLOW__SET_AT_TRUE) {
    // arrange
    Bitmap bmp = {0b1001};

    // act
    bmp.set(3);

    // assert
    ASSERT_EQ(bmp.data, (Bitmap) {0b1001}.data);
}

TEST(BITMAP, HAPPY_FLOW__UNSET_AT_TRUE) {
    // arrange
    Bitmap bmp = {0b10101};

    // act
    bmp.unset(2);

    // assert
    ASSERT_EQ(bmp.data, (Bitmap) {0b10001}.data);
}

TEST(BITMAP, HAPPY_FLOW__UNSET_AT_FALSE) {
    // arrange
    Bitmap bmp = {0b10101};

    // act
    bmp.unset(3);

    // assert
    ASSERT_EQ(bmp.data, (Bitmap) {0b10101}.data);
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

    auto *c1 = new Hamt<int, int>::CNode;
    auto *k = new Hamt<int, int>::SNode(4, 5);
    c1->insertChild(k, 4);

    // act
    auto *c2 = new Hamt<int,int>::CNode(*c1);

    // assert
    ASSERT_NE(c1, c2);
    ASSERT_EQ(c1->array[0], c2->array[0]);
    ASSERT_EQ(c1->bmp.data, c2->bmp.data);
}

TEST(CNode, HAPPY_FLOW_GET__NODE_LOOKUP_NOT_FOUND) {
    // arrange
    Hamt<string, int>::CNode node;
    node.bmp = {0b100001};

    // assert
    ASSERT_EQ(node.getSubNode(0b11111), nullptr);
}

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> i2 -> c2 -> i3 -> c3 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TO_EMPTY_TRIE) {
    // arrange
    Hamt<string, int> hamt;

    // act
    hamt.insert("abc", 1);

    // assert
    ASSERT_NE(hamt.root->main.load()->getSubNode(8), nullptr);
    ASSERT_EQ(hamt.root->main.load()->getSubNode(0), nullptr);
}

TEST(TRIE, HAPPY_FLOW__INSERT_TWO_KEYS_WITH_EQUAL_HASH) {
    // arrange
    Hamt<string, int> hamt;

    // act
    auto equalHash = findTwoKeyWithEqualHash();
    hamt.insert(equalHash.first, 1);
    hamt.insert(equalHash.second, 2);



    ASSERT_EQ(hamt.lookup(equalHash.first), (hamt.createSuccessfulLookupResult(1)));
    ASSERT_EQ(hamt.lookup(equalHash.second), (hamt.createSuccessfulLookupResult(2)));
}

TEST(TRIE, HAPPY_FLOW__CONTRACTED_CHECK_WITH_THREE_KEYS) {
    // arrange
    Hamt<int, int> hamt;

    // act & act
    hamt.insert(0b00000, 1);
    hamt.insert(0b00001, 2);
    hamt.insert(0b100001, 3);
    ASSERT_EQ(hamt.root->main.load()->getChildCount(), 2);
    hamt.remove(0b100001);


    ASSERT_EQ(hamt.root->main.load()->array[0]->type, SNODE);
    ASSERT_EQ(hamt.root->main.load()->getChildCount(), 2);
    ASSERT_EQ(hamt.root->main.load()->array[1]->type, SNODE);
}
//
//TEST(TRIE, HAPPY_FLOW__CONTRACTED_CHECK_WITH_MANY_KEYS) {
//    // arrange
//
//    // act
//
//    // assert
//}

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

    ASSERT_EQ(hamt.lookup("k223"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k224"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k225"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k226"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k227"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k228"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k229"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2210"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2211"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2212"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2213"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2214"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2215"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2216"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2217"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2218"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2229"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2220"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2222"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2223"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2224"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2225"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2226"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k227"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2228"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k22d29"), (hamt.LOOKUP_NOT_FOUND));

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
    ASSERT_EQ(hamt.remove("k1"), (hamt.createSuccessfulRemoveResult(1)));
    ASSERT_EQ(hamt.remove("k2"), (hamt.createSuccessfulRemoveResult(2)));
    ASSERT_EQ(hamt.remove("k30"), (hamt.createSuccessfulRemoveResult(30)));
    ASSERT_EQ(hamt.remove("k3"), (hamt.createSuccessfulRemoveResult(3)));
    ASSERT_EQ(hamt.remove("k4"), (hamt.createSuccessfulRemoveResult(4)));
    ASSERT_EQ(hamt.remove("k5"), (hamt.REMOVE_NOT_FOUND));
    ASSERT_EQ(hamt.remove("k7"), (hamt.REMOVE_NOT_FOUND));

    // assert
    ASSERT_EQ(hamt.lookup("k1"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k2"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k3"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k30"), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_EQ(hamt.lookup("k4"), (hamt.LOOKUP_NOT_FOUND));

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
        ASSERT_EQ(hamt.lookup(i), (hamt.createSuccessfulLookupResult(i)));
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(hamt.remove(i), (hamt.createSuccessfulRemoveResult(i)));
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }

}


TEST(TRIE, HAPPY_FLOW__INSERTING_AND_LOOKING_UP_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                hamt->insert(i, i);
            }
            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.createSuccessfulLookupResult(i)));
    }
}


TEST(TRIE, HAPPY_FLOW__INSERTING_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                hamt->insert(i, i);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.createSuccessfulLookupResult(i)));
    }
}

TEST(TRIE, HAPPY_FLOW__LOOKING_UP_KEYS_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;

    for (int i = averageIterationCount; i < averageIterationCount * 10; i++) {
        hamt.insert(i, i);
    }

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                assert(hamt->lookup(i) == (hamt->LOOKUP_NOT_FOUND));
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }
    for (int i = averageIterationCount; i < averageIterationCount * 10; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.createSuccessfulLookupResult(i)));
    }
}



TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_RANDOM_KEY_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                int k = rand() % 100;
                hamt->insert(k, k);
                hamt->remove(k);
            }
            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }
}

TEST(TRIE, HAPPY_FLOW__KEY_INSERTING_AND_REMOVING_SEQUENTALLY_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                hamt->insert(i, i);
                assert(hamt->remove(i) == (hamt->createSuccessfulRemoveResult(i)));
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }
}

TEST(TRIE, HAPPY_FLOW__KEY_RANDOM_ACTIONS_BY_MANY_THREAD) {
    // arrange & act
    Hamt<int, int> hamt;
    int threadCount = rand() % 8 + 2;
    int averageIterationCount = rand() % 50000;

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                switch (rand() % 3) {
                    case 0: {
                        hamt->insert(i, i);
                        break;
                    }
                    case 1: {
                        hamt->remove(i);
                        break;
                    }
                    case 2: {
                        hamt->lookup(i);
                        break;
                    }
                }
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }
    for (int i = 0; i < averageIterationCount; i++) {
        hamt.remove(i);
    }

    // assert
    for (int i = 0; i < averageIterationCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }
}


TEST(TRIE, HAPPY_FLOW__RANDOM_REMOVING_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 5;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
            set<int> keys;
            for (int i = (*id) * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                keys.insert(i);
                hamt->insert(i, i);
            }
            while (!keys.empty()) {
                int key = rand() % (*averageIterationCount) + (*id) * (*averageIterationCount);
                if (hamt->remove(key) != (Hamt<int,int>::createSuccessfulRemoveResult(key))) {
                    keys.erase(key);
                }
            }


            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }
}


TEST(TRIE, HAPPY_FLOW__INSERTING_THE_SAME_KEY_MANY_TIMES_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 1; i < averageIterationCount * threadCount / 2; i++) {
        hamt.insert(i, i);
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = 0; i < *averageIterationCount; i++) {
                hamt->insert(0, *id);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    ASSERT_NE(hamt.lookup(0), (hamt.LOOKUP_NOT_FOUND));
    ASSERT_LT(hamt.lookup(0).value, thread.size());
    ASSERT_GE(hamt.lookup(0).value, 0);
}

TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_ONE_ELEMENT_BY_MANY_THREAD) {
    // arrange
    Hamt<int, int> hamt;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&hamt, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *hamt = (Hamt<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                assert(hamt->insert(i, i) != (hamt->INSERT_RESTART));
                assert(hamt->lookup(i) == (hamt->createSuccessfulLookupResult(i)));
                assert(hamt->remove(i) == (hamt->createSuccessfulRemoveResult(i)));
                assert(hamt->lookup(i) == (hamt->LOOKUP_NOT_FOUND));
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(hamt.lookup(i), (hamt.LOOKUP_NOT_FOUND));
    }
}




