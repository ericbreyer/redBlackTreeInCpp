#include "./IComparable.h"
#include "IMap/IMap.h"
#include "bloomFilter/bloomGuard.cpp"
#include "linkedHashTable/linkedHashTable.h"
#include "probeHashTable/probeHashTable.h"
#include "rbtree/rbtree.h"
#include <assert.h>
#include <cstdlib>
#include <iostream>
class funnyInt : public IComparable {
private:
    int data;

public:
    funnyInt(int v) {
        data = v;
    };
    int getIntRep() override {
        return data;
    }
    int operator-(IComparable *rhs) override {
        return data - rhs->getIntRep();
    };
    bool operator==(IComparable *rhs) override {
        return data - rhs->getIntRep() == 0;
    };
};

int fib(int n, IMap<int, int> *memo) {
    if (n <= 1) {
        (*memo)[n] = n;
    } else if (!memo->contains(n)) {
        (*memo)[n] = fib(n - 1, memo) + fib(n - 2, memo);
    }
    return (*memo).getValue(n);
}

int main(int arcv, char *argv[]) {

    IMap<int, int> *memo = new probeHashTable<int, int>();
    IMap<int, int> *memo2 = new linkedHashTable<int, int>();
    IMap<int, int> *memo3 = new rbtree<int, int>();
    IMap<int, int> *memo4 = bloomGuard<int, int>::bloomGuardFactory(bloomGuard<int, int>::RED_BLACK_TREE, 1000);
    IMap<int, int> *test;

    switch (std::stoi(argv[1])) {
    case 1:
        test = new linkedHashTable<int, int>();
        break;
    case 2:
        test = new rbtree<int, int>();
        break;
    case 3:
        test = new probeHashTable<int, int>();
        break;
    case 4:
        test = bloomGuard<int, int>::bloomGuardFactory(bloomGuard<int, int>::LINKED_HASH_TABLE, 20000);
        break;
    case 5:
        test = bloomGuard<int, int>::bloomGuardFactory(bloomGuard<int, int>::RED_BLACK_TREE, 20000);
        break;
    case 6:
        test = bloomGuard<int, int>::bloomGuardFactory(bloomGuard<int, int>::PROBED_HASH_TABLE, 20000);
        break;
    default:
        break;
    }

    int itrs = std::stoi(argv[2]);

    for (int i = 0; i < itrs; ++i) {
        int opp = (int)(((float)std::rand() / (float)RAND_MAX) * 3);
        int key = (int)(((float)std::rand() / (float)RAND_MAX) * itrs / 2);
        int val = 1;
        switch (opp) {
        case 0:
            test->insert(key, val);
            break;
        case 1:
        case 2:
            test->contains(key);
            break;
        default:
            break;
        }
    }

    delete test;
}