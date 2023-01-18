#include "./IComparable.h"
#include "IMap/IMap.h"
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

int main() {
    // IMap<int, int> *memo2 = new linkedHashTable<int, int>();
    IMap<int, int> *memo3 = new rbtree<int, int>();

    std::cout << fib(10, memo3) << std::endl;
    return 0;
}