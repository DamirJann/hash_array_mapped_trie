
#pragma once
#define protected public
#define private   public

#include "../include/trie.h"

#undef protected
#undef private

#include <gtest/gtest.h>
#include <random>

int averageIterationCount = 10000;

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

TEST(CNode, HAPPY_FLOW__GET_COPY) {
    // arrange

    auto *c1 = new CNode<int, int>();
    auto *k = new SNode<int, int>(4, 5);
    c1->insertChild(k, 4);

    // act
    CNode<int, int> *c2 = getCopy(c1);

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

// BEFORE: i1 -> c1
//  AFTER: i1 -> c1 -> i2 -> c2 -> i3 -> c3 -> k1, k2
TEST(TRIE, HAPPY_FLOW__INSERT_TO_EMPTY_TRIE) {
    // arrange
    Trie<string, int> trie;

    // act
    trie.insert("abc", 1);

    // assert
    ASSERT_NE(trie.root->main.load()->getSubNode(8), nullptr);
    ASSERT_EQ(trie.root->main.load()->getSubNode(0), nullptr);
}

TEST(TRIE, HAPPY_FLOW__LOOKUP) {
    // arrange
    Trie<string, int> trie;

    trie.insert("k1", 1);
    trie.insert("k2", 2);
    trie.insert("k3", 3);
    trie.insert("k4", 4);
    trie.insert("k5", 5);
    trie.insert("k6", 6);
    trie.insert("k7", 7);
    trie.insert("k8", 8);
    trie.insert("k9", 9);
    trie.insert("k10", 10);
    trie.insert("k11", 11);
    trie.insert("k12", 12);
    trie.insert("k13", 13);
    trie.insert("k14", 14);
    trie.insert("k15", 15);
    trie.insert("k16", 16);
    trie.insert("k17", 17);
    trie.insert("k18", 18);
    trie.insert("k19", 19);
    trie.insert("k20", 20);
    trie.insert("k21", 21);
    trie.insert("k22", 22);
    trie.insert("k23", 23);
    trie.insert("k24", 24);
    trie.insert("k25", 25);

    // act & assert
    ASSERT_EQ(trie.lookup("k1").value, 1);
    ASSERT_EQ(trie.lookup("k2").value, 2);
    ASSERT_EQ(trie.lookup("k3").value, 3);
    ASSERT_EQ(trie.lookup("k4").value, 4);
    ASSERT_EQ(trie.lookup("k5").value, 5);
    ASSERT_EQ(trie.lookup("k6").value, 6);
    ASSERT_EQ(trie.lookup("k7").value, 7);
    ASSERT_EQ(trie.lookup("k8").value, 8);
    ASSERT_EQ(trie.lookup("k9").value, 9);
    ASSERT_EQ(trie.lookup("k10").value, 10);
    ASSERT_EQ(trie.lookup("k11").value, 11);
    ASSERT_EQ(trie.lookup("k12").value, 12);
    ASSERT_EQ(trie.lookup("k13").value, 13);
    ASSERT_EQ(trie.lookup("k14").value, 14);
    ASSERT_EQ(trie.lookup("k15").value, 15);
    ASSERT_EQ(trie.lookup("k16").value, 16);
    ASSERT_EQ(trie.lookup("k17").value, 17);
    ASSERT_EQ(trie.lookup("k18").value, 18);
    ASSERT_EQ(trie.lookup("k19").value, 19);
    ASSERT_EQ(trie.lookup("k20").value, 20);
    ASSERT_EQ(trie.lookup("k21").value, 21);
    ASSERT_EQ(trie.lookup("k22").value, 22);
    ASSERT_EQ(trie.lookup("k23").value, 23);
    ASSERT_EQ(trie.lookup("k24").value, 24);
    ASSERT_EQ(trie.lookup("k25").value, 25);

    ASSERT_EQ(trie.lookup("k223"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k224"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k225"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k226"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k227"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k228"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k229"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2210"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2211"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2212"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2213"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2214"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2215"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2216"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2217"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2218"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2229"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2220"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2222"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2223"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2224"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2225"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2226"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k227"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2228"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k22d29"), LOOKUP_NOT_FOUND);

}

TEST(TRIE, HAPPY_FLOW__REMOVE_ALL_KEYS) {
    // arrange
    Trie<string, int> trie;
    trie.insert("k1", 1);
    trie.insert("k2", 2);
    trie.insert("k3", 3);
    trie.insert("k30", 30);
    trie.insert("k4", 4);

    // act
    ASSERT_EQ(trie.remove("k1"), createSuccessfulRemoveResult(1));
    ASSERT_EQ(trie.remove("k2"), createSuccessfulRemoveResult(2));
    ASSERT_EQ(trie.remove("k30"), createSuccessfulRemoveResult(30));
    ASSERT_EQ(trie.remove("k3"), createSuccessfulRemoveResult(3));
    ASSERT_EQ(trie.remove("k4"), createSuccessfulRemoveResult(4));
    ASSERT_EQ(trie.remove("k5"), REMOVE_NOT_FOUND);
    ASSERT_EQ(trie.remove("k7"), REMOVE_NOT_FOUND);

    // assert
    ASSERT_EQ(trie.lookup("k1"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k2"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k3"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k30"), LOOKUP_NOT_FOUND);
    ASSERT_EQ(trie.lookup("k4"), LOOKUP_NOT_FOUND);

}

TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_KEYS_BY_ONE_THREAD) {
    // arrange
    Trie<int, int> trie;
    int count = 100000;

    // act & assert
    for (int i = 0; i < count; i++) {
        trie.insert(i, i);
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(trie.lookup(i), createSuccessfulLookupResult(i));
    }

    for (int i = 0; i < count; i++) {
        ASSERT_EQ(trie.remove(i), createSuccessfulRemoveResult(i));
    }

    // assert
    for (int i = 0; i < count; i++) {
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }

}


TEST(TRIE, HAPPY_FLOW__INSERTING_AND_LOOKING_UP_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                trie->insert(i, i);
            }
            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
//        cout << i << endl;
        ASSERT_EQ(trie.lookup(i), createSuccessfulLookupResult(i));
    }
}


TEST(TRIE, HAPPY_FLOW__INSERTING_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                trie->insert(i, i);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(trie.lookup(i), createSuccessfulLookupResult(i));
    }
}

TEST(TRIE, HAPPY_FLOW__LOOKING_UP_KEYS_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;

    for (int i = averageIterationCount; i < averageIterationCount * 10; i++) {
        trie.insert(i, i);
    }

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                assert(trie->lookup(i) == LOOKUP_NOT_FOUND);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount; i++) {
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }
    for (int i = averageIterationCount; i < averageIterationCount * 10; i++) {
        ASSERT_EQ(trie.lookup(i), createSuccessfulLookupResult(i));
    }
}


TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_ONE_ELEMENT_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                trie->insert(i, i);
                assert(trie->lookup(i) == createSuccessfulLookupResult(i));
                trie->remove(i);
                assert(trie->lookup(i) == LOOKUP_NOT_FOUND);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }
}

TEST(TRIE, HAPPY_FLOW__INSERTING_AND_REMOVING_RANDOM_KEY_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                int k = rand() % 100;
                trie->insert(k, k);
                trie->remove(k);
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
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = *id * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                trie->insert(i, i);
                assert(trie->remove(i) == createSuccessfulRemoveResult(i));
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    for (int i = 0; i < averageIterationCount * threadCount; i++) {
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }
}

TEST(TRIE, HAPPY_FLOW__KEY_RANDOM_ACTIONS_BY_MANY_THREAD) {
    // arrange & act
    Trie<int, int> trie;
    int threadCount = rand() % 8 + 2;
    int averageIterationCount = rand() % 50000;

    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[1];

            for (int i = 0; i < *averageIterationCount; i++) {
                switch (rand() % 3) {
                    case 0: {
                        trie->insert(i, i);
                        break;
                    }
                    case 1: {
                        trie->remove(i);
                        break;
                    }
                    case 2: {
                        trie->lookup(i);
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
        trie.remove(i);
    }

    // assert
    for (int i = 0; i < averageIterationCount; i++) {
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }
}


TEST(TRIE, HAPPY_FLOW___RANDOM_REMOVING_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];
            set<int> keys;
            for (int i = (*id) * (*averageIterationCount); i < (*id + 1) * (*averageIterationCount); i++) {
                keys.insert(i);
                trie->insert(i, i);
            }
            while (!keys.empty()) {
                int key = rand() % (*averageIterationCount) + (*id) * (*averageIterationCount);
                if (trie->remove(key) != createSuccessfulRemoveResult(key)) {
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
        ASSERT_EQ(trie.lookup(i), LOOKUP_NOT_FOUND);
    }
}


TEST(TRIE, HAPPY_FLOW___INSERTING_THE_SAME_KEY_MANY_TIMES_BY_MANY_THREAD) {
    // arrange
    Trie<int, int> trie;
    int threadCount = 10;


    vector<pthread_t> thread(threadCount);
    vector<vector<void *>> attr(threadCount);
    for (int i = 0; i < attr.size(); i++) {
        attr[i] = {&trie, new int(i), new int(averageIterationCount)};
    }

    for (int i = 1; i < 100000; i++) {
        trie.insert(i, i);
    }

    for (int i = 0; i < thread.size(); i++) {
        pthread_create(&thread[i], nullptr, [](void *args) -> void * {
            auto *trie = (Trie<int, int> *) (*static_cast<vector<void *> *>(args))[0];
            int *id = (int *) (*static_cast<vector<void *> *>(args))[1];
            int *averageIterationCount = (int *) (*static_cast<vector<void *> *>(args))[2];

            for (int i = 0; i < *averageIterationCount; i++) {
                trie->insert(0, *id);
            }

            pthread_exit(nullptr);
        }, &attr[i]);

    }

    for (unsigned long i: thread) {
        pthread_join(i, nullptr);
    }

    // assert
    ASSERT_EQ(trie.lookup(0).status, LookupResult::Found);
    ASSERT_LT(trie.lookup(0).value, thread.size());
    ASSERT_GE(trie.lookup(0).value, 0);
}



