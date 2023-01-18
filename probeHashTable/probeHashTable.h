#pragma once

#include "../IMap/IMap.h"
#include <string>

template <typename K, typename V = int>
class probeHashTable : public IMap<K, V> {
private:
    struct kvPair {
        K key;
        V value;
        bool empty = true;
    };

    static unsigned int hash(unsigned int x);
    static unsigned int polyProbe(unsigned int x);

    int numSlots;
    int numKeys;
    float alphaMax = .75;
    kvPair *underlyingArray;

    float loadFactor();

    void resize(float factor);

public:
    probeHashTable(int numSlots = 8);
    void print() override;
    int getKeys(K *keys) override;
    void clear() override;
    IMap<K, V> *copy() override;
    bool contains(K key) override;
    V getValue(K key) override;
    bool tryGetValue(K key, V *out) override;
    bool setValue(K key, V value) override;
    bool insert(K key, V value) override;
    typename IMap<K, V>::getSetProxy operator[](K key) override;
    bool remove(K key) override;
    ~probeHashTable();
};