#include <cstdint>
#include <string>
#include <vector>
#include <bitset>
#include "utils.h"
#include <set>
#include <mutex>
#include "atomic"

#pragma once
using namespace std;


enum NodeType {
    CNODE,
    SNODE,
    INODE
};

template<class K, class V>
class Hamt {
public:
    Hamt() {
        root = new INode(nullptr);
    }

    class Node {
    public:
        NodeType type;
    protected:

        Node(NodeType type) {
            this->type = type;
        }
    };

    class SNode : public Node {
    public:

        SNode(K k, V v) : Node(SNODE) {
            this->pair.insert({k, v});
            this->hash = generateSimpleHash(k);
        }

        uint64_t getHash() {
            return hash;
        }

        bool contains(K k) {
            for (auto &p: this->pair) {
                if (p.key == k) {
                    return true;
                }
            }
            return false;
        }

        bool contains(SNode *node) {
            for (auto &p: node->pair) {
                if (this->contains(p.key)) {
                    return true;
                }
            }
            return false;
        }

        int getValue(K k) {
            for (auto &p: this->pair) {
                if (p.key == k) {
                    return p.value;
                }
            }
            return 0;
        }

        struct Pair {
            K key;
            V value;

            bool operator<(const Pair &p) const {
                return this->key < p.key;
            }
        };

    public:
        uint64_t hash{};
        set<Pair> pair;
    };

    class CNode : public Node {
    public:
        bool isTomb;

        CNode() : Node(CNODE) {
            isTomb = false;
        }

        Node *getSubNode(uint8_t path) const {
            if (!bmp.isSet(path)) return nullptr;
            int index = getArrayIndexByBmp(path);
            return array[index];
        }

        uint8_t getChildCount() const {
            return __builtin_popcount(bmp.data);
        }

        Node *getFirstChild() const {
            return array.front();
        }

        void insertChild(Node *const newChild, uint8_t path) {
            bmp.set(path);
            array.insert(array.begin() + getArrayIndexByBmp(path), newChild);
        }

        void replaceChild(Node *const newChild, uint8_t path) {
            array[getArrayIndexByBmp(path)] = newChild;
        }

        void deleteChild(uint8_t path) {
            array.erase(array.begin() + getArrayIndexByBmp(path));
            bmp.unset(path);
        }

        uint8_t getArrayIndexByBmp(uint8_t pos) const {
            return __builtin_popcount(
                    ((1 << pos) - 1) & bmp.data
            );
        }

    private:
        Bitmap bmp{};
        vector<Node *> array;
    };

    class INode : public Node {
    public:
        INode() : Node(INODE) {}

        INode(CNode *main) : INode() {
            this->main.store(main, std::memory_order_seq_cst);
        }

        atomic<CNode *> main;
    };


    struct InsertResult {
        enum class Status {
            Inserted,
            Failed
        };

        V value;
        const Status status;

        bool operator==(InsertResult b) const {
            if ((status == Status::Failed) && (b.status == Status::Failed)) return true;
            if ((status == Status::Inserted) && (b.status == Status::Inserted)) return true;
            return false;
        }

        bool operator!=(InsertResult b) const {
            return !(*this == b);
        }
    };

    struct LookupResult {
        enum class Status {
            NotFound,
            Found,
            Failed
        };

        V value;
        Status status;

        bool operator==(LookupResult b) const {
            if ((status == Status::NotFound) && (b.status == Status::NotFound)) return true;
            if ((status == Status::Failed) && (b.status == Status::Failed)) return true;
            if ((status == Status::Found) && (b.status == Status::Found)) return this->value == b.value;
            return false;
        }

        bool operator!=(LookupResult b) const {
            return !(*this == b);
        }
    };

    const InsertResult INSERT_RESTART{.status = InsertResult::Status::Failed};
    const InsertResult INSERT_SUCCESSFUL{.status = InsertResult::Status::Inserted};

    LookupResult createSuccessfulLookupResult(V value) {
        return {value, LookupResult::Status::Found};
    }

    const LookupResult LOOKUP_NOT_FOUND{.status = LookupResult::Status::NotFound};
    const LookupResult LOOKUP_RESTART{.status = LookupResult::Status::Failed};

    struct RemoveResult {
        enum class Status {
            Removed,
            Failed,
            NotFound,
        };

        int value;
        Status status;

        bool operator==(RemoveResult rightOperand) const {
            if ((status == Status::NotFound) && (rightOperand.status == Status::NotFound)) return true;
            if ((status == Status::Failed) && (rightOperand.status == Status::Failed)) return true;
            if ((status == Status::Removed) && (rightOperand.status == Status::Removed))
                return value == rightOperand.value;
            return false;
        }

        bool operator!=(RemoveResult rightOperand) const {
            return !(*this == rightOperand);
        }
    };

    const RemoveResult REMOVE_NOT_FOUND{.status = RemoveResult::Status::NotFound};
    const RemoveResult REMOVE_RESTART{.status = RemoveResult::Status::Failed};

    static RemoveResult createSuccessfulRemoveResult(int value) {
        return {value, RemoveResult::Status::Removed};
    }

    SNode *leftMerge(SNode *node1, SNode *node2) {
        auto *merged = new SNode(*node1);
        for (auto &p: node2->pair) {
            if (!merged->contains(p.key)) {
                merged->pair.insert(p);
            }
        }
        return merged;
    }

    static CNode *getCopy(CNode *node) {
        return new CNode(*node);
    }

    void transformToContractedParent(CNode *updated, CNode *m, uint8_t path) {
        updated->replaceChild(m->getFirstChild(), path);
    }

    void transformToWithReplacedPair(CNode *updated, SNode *subNode, SNode *newNode, uint8_t path) {
        updated->replaceChild(leftMerge(newNode, subNode), path);
    }

    void transformToWithInsertedChild(CNode *updated, Node *child, uint8_t path) {
        updated->insertChild(child, path);
    }

    void transformToWithMergedChild(CNode *updated, SNode *subNode, SNode *newNode, uint8_t path) {
        updated->replaceChild(leftMerge(newNode, subNode), path);
    }

    void
    transformToWithDownChild(CNode *updated, SNode *newChild, SNode *oldChild, uint8_t level, uint8_t path) {

        if (newChild->getHash() == oldChild->getHash()) {
            newChild = leftMerge(newChild, oldChild);
            updated->replaceChild(newChild, path);
        } else {

            auto *cur_c = new CNode();
            auto *i = new INode(cur_c);

            int j = level + 1;
            uint8_t newChildHashPath = extractHashPartByLevel(newChild->getHash(), j);
            uint8_t oldChildHashPath = extractHashPartByLevel(oldChild->getHash(), j);

            while (newChildHashPath == oldChildHashPath) {
                auto *c = new CNode();
                cur_c->insertChild(new INode(c), oldChildHashPath);
                cur_c = c;
                j++;
                newChildHashPath = extractHashPartByLevel(newChild->getHash(), j);
                oldChildHashPath = extractHashPartByLevel(oldChild->getHash(), j);


            }
            cur_c->insertChild(newChild, newChildHashPath);
            cur_c->insertChild(oldChild, oldChildHashPath);

            updated->replaceChild(i, path);
        }

    }

    void transformToWithDeletedKey(CNode *updated, SNode *subNode, K key, uint8_t path) {
        auto *newSubNode = new SNode(*subNode);
        if (newSubNode->pair.size() > 1) {
            newSubNode->pair.erase({key, subNode->getValue(key)});
            updated->replaceChild(newSubNode, path);
        } else {
            updated->deleteChild(path);
        }
    }

    bool isTombed(const CNode *const c, const INode *const root, const INode *const parent) {
        return root != parent &&
               c->getChildCount() == 1 &&
               c->getFirstChild()->type == SNODE;
    }

    bool
    contractParent(INode *parent, INode *i, CNode *pm, CNode *m, uint8_t level, uint64_t hash) {
        if (!m->isTomb) return false;

        if (pm->getSubNode(extractHashPartByLevel(hash, level - 1)) != i) {
            return true;
        }

        CNode *updated = getCopy(pm);
        transformToContractedParent(updated, m, extractHashPartByLevel(hash, level - 1));
        parent->main.compare_exchange_strong(pm, updated);
        return true;
    }

    Node *getRoot() {
        return this->root;
    }

    LookupResult lookup(K key) {
        while (true) {
            if (root->main.load() == nullptr) {
                return LOOKUP_NOT_FOUND;
            } else {
                LookupResult res = lookup(root, nullptr, key, generateSimpleHash(key), 0);
                if (res != LOOKUP_RESTART) {
                    return res;
                }
            }
        }
    }

    RemoveResult remove(K key) {
        while (true) {
            if (root->main.load() == nullptr) {
                return REMOVE_NOT_FOUND;
            }
            RemoveResult res = remove(root, nullptr, key, generateSimpleHash(key), 0);
            if (res != REMOVE_RESTART) {
                return res;
            }
        }
    }

    InsertResult insert(K key, V value) {
        while (true) {
            CNode *old = root->main.load();
            if (old == nullptr) {
                auto *c = new CNode();
                auto *s = new SNode(key, value);
                c->insertChild(s, extractHashPartByLevel(s->getHash(), 0));
                if (root->main.compare_exchange_strong(old, c)) {
                    return InsertResult{.status = InsertResult::Status::Inserted};
                }
            } else {
                InsertResult res = insert(root, nullptr, new SNode(key, value), 0);
                if (res != INSERT_RESTART) {
                    return res;
                }
            }
        }
    }


private:
    INode *root;

    LookupResult lookup(INode *currentNode, INode *parent, K key, uint64_t hash, uint8_t level) {
        CNode *pm = parent ? parent->main.load() : nullptr;
        CNode *m = currentNode->main.load();

        if (contractParent(parent, currentNode, pm, m, level, hash)) {
            return LOOKUP_RESTART;
        }

        Node *nextNode = m->getSubNode(extractHashPartByLevel(hash, level));
        if (nextNode == nullptr) {
            return LOOKUP_NOT_FOUND;
        } else if (nextNode->type == SNODE) {
            if (static_cast<SNode *>(nextNode)->contains(key)) {
                return createSuccessfulLookupResult(static_cast<SNode *>(nextNode)->getValue(key));
            }
            return LOOKUP_NOT_FOUND;
        } else if (nextNode->type == INODE) {
            return lookup(static_cast<INode *>(nextNode), currentNode, key, hash, level + 1);
        }
    }

    RemoveResult remove(INode *currentNode, INode *parent, K key, uint64_t hash, uint8_t level) {
        CNode *pm = parent ? parent->main.load() : nullptr;
        CNode *m = currentNode->main.load();

        if (contractParent(parent, currentNode, pm, m, level, hash)) {
            return REMOVE_RESTART;
        }

        CNode *updated = getCopy(m);
        uint8_t path = extractHashPartByLevel(hash, level);
        Node *subNode = updated->getSubNode(path);

        RemoveResult res{};

        if (subNode == nullptr) {
            res = REMOVE_NOT_FOUND;
        } else if (subNode->type == SNODE) {
            if (static_cast<SNode *>(subNode)->contains(key)) {
                V delVal = static_cast<SNode *>(subNode)->getValue(key);
                transformToWithDeletedKey(updated, static_cast<SNode *>(subNode), key,
                                          extractHashPartByLevel(hash, level));
                updated->isTomb = isTombed(updated, root, currentNode);
                res = (currentNode->main.compare_exchange_strong(m, updated))
                      ? createSuccessfulRemoveResult(delVal) : REMOVE_RESTART;
            } else {
                res = REMOVE_NOT_FOUND;
            }
        } else if (subNode->type == INODE) {
            res = remove(static_cast<INode *>(subNode), currentNode, key, hash, level + 1);
        }

        if (res == REMOVE_NOT_FOUND || res == REMOVE_RESTART) {
            return res;
        }

        contractParent(parent, currentNode, pm, updated, level, hash);

        return res;
    }

    InsertResult insert(INode *currentNode, INode *parent, SNode *newNode, uint8_t level) {
        CNode *pm = parent ? parent->main.load() : nullptr;
        CNode *m = currentNode->main.load();

        if (contractParent(parent, currentNode, pm, m, level, newNode->getHash())) {
            return INSERT_RESTART;
        }

        CNode *updated = getCopy(m);
        uint8_t path = extractHashPartByLevel(newNode->getHash(), level);


        Node *subNode = updated->getSubNode(path);
        if (subNode == nullptr) {
            transformToWithInsertedChild(updated, newNode, path);
            updated->isTomb = isTombed(updated, root, currentNode);
            return currentNode->main.compare_exchange_strong(m, updated) ? INSERT_SUCCESSFUL : INSERT_RESTART;
        } else if (subNode->type == SNODE) {
            auto *s = static_cast<SNode *>(subNode);
            if (s->contains(newNode)) {
                transformToWithReplacedPair(updated, s, newNode, path);
                updated->isTomb = isTombed(updated, root, currentNode);
                return currentNode->main.compare_exchange_strong(m, updated) ? INSERT_SUCCESSFUL : INSERT_RESTART;
            } else if (level == MAX_LEVEL_COUNT) {
                transformToWithMergedChild(updated, s, newNode, path);
                updated->isTomb = isTombed(updated, root, currentNode);
                return currentNode->main.compare_exchange_strong(m, updated) ? INSERT_SUCCESSFUL : INSERT_RESTART;
            } else {
                transformToWithDownChild(updated, newNode, s, level, path);
                updated->isTomb = isTombed(updated, root, currentNode);
                return currentNode->main.compare_exchange_strong(m, updated) ? INSERT_SUCCESSFUL : INSERT_RESTART;
            }
        } else if (subNode->type == INODE) {
            return insert(static_cast<INode *>(subNode), currentNode, newNode, level + 1);
        } else {
            fprintf(stderr, "Node with unknown type\n");
            assert(false);
            return INSERT_RESTART;
        }
    }
};