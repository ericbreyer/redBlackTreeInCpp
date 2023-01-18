#include "../IMap/IMap.cpp"
#include "../IMap/IMap.h"
#include "../linkedHashTable/linkedHashTable.cpp"
#include "../linkedHashTable/linkedHashTable.h"
#include "../probeHashTable/probeHashTable.cpp"
#include "../probeHashTable/probeHashTable.h"
#include "../rbtree/rbtree.cpp"
#include "../rbtree/rbtree.h"
#include "./bloomFilter.cpp"
#include <iostream>

#pragma once

template <typename K, typename V>
class bloomGuard : public IMap<K, V> {
private:
    IMap<K, V> *map;
    bloomFilter<K> *filter;

    bloomGuard(IMap<K, V> *map, int elems = 10) : map(map), filter(new bloomFilter<K>(elems)){};

public:
    enum innerMapImpl {
        RED_BLACK_TREE,
        PROBED_HASH_TABLE,
        LINKED_HASH_TABLE
    };

    static IMap<K, V> *bloomGuardFactory(bloomGuard<K, V>::innerMapImpl impl, int elems = 10) {
        switch (impl) {
        case bloomGuard<K, V>::innerMapImpl::RED_BLACK_TREE:
            return new bloomGuard<K, V>(new rbtree<K, V>(), elems);
            break;
        case bloomGuard<K, V>::innerMapImpl::PROBED_HASH_TABLE:
            return new bloomGuard<K, V>(new probeHashTable<K, V>(), elems);
            break;
        case bloomGuard<K, V>::innerMapImpl::LINKED_HASH_TABLE:
            return new bloomGuard<K, V>(new linkedHashTable<K, V>(), elems);
            break;
        default:
            return nullptr;
            break;
        }
    }

    void print() {
        map->print();
    };
    int getKeys(K *keys) {
        return map->getKeys(keys);
    }
    void clear() {
        map->clear();
    };
    IMap<K, V> *copy() {
        return map->copy();
    };
    bool contains(K key) {
        if (filter->contains(key) == DEFINITELY_NOT) {
            return false;
        }
        return map->contains(key);
    };
    V getValue(K key) {
        if (filter->contains(key) == DEFINITELY_NOT) {
            return 0;
        }
        return map->getValue(key);
    };
    bool tryGetValue(K key, V *out) {
        if (filter->contains(key) == DEFINITELY_NOT) {
            return false;
        }
        return map->tryGetValue(key, out);
    };
    bool setValue(K key, V value) {
        if (filter->contains(key) == DEFINITELY_NOT) {
            return false;
        }
        return map->setValue(key, value);
    };
    bool insert(K key, V value = 0) {
        bool success = map->insert(key, value);
        if (success) {
            filter->insert(key);
        }
        return success;
    };
    typename IMap<K, V>::getSetProxy operator[](K key) {
        return typename IMap<K, V>::getSetProxy(this, key);
    };
    bool remove(K key) {
        if (filter->contains(key) == DEFINITELY_NOT) {
            return false;
        }
        bool success = map->remove(key);
        if (success) {
            filter->remove(key);
        }
        return success;
    };
    ~bloomGuard() {
        delete map;
        delete filter;
    };
};