#include <boost/graph/graphviz.hpp>
#include "../include/utils.h"

uint64_t generateHash(string k) {
    uint64_t hash = 0;
    for (size_t i = 0; i < k.size(); i++) {
        hash += k[i] * i;
    }
    return hash % 1000;
}