#pragma once

#include "../IMap/IMap.h"
#include <iostream>
#include <string>

template <typename K, typename V = int>
class linkedHashTable : public IMap<K, V> {
public:
    class chainingList {

    public:
        class node {
        public:
            K key;
            V value;
            node *next;
            node(K key, V value);
            ~node();
        };
        chainingList::node *root;
        chainingList();
        bool insert(K key, V value);
        bool setValue(K key, V value);
        bool remove(K key);
        bool tryGet(K key, V *out);
        int keys(K *keys);
        std::string toString();
        ~chainingList();
    };

    // linkedHashTable(linkedHashTable &obj);

    static unsigned int hash(unsigned int x);

    int numSlots;
    int numKeys;
    float alphaMax = 100;
    chainingList *underlyingArray;

    float loadFactor();

    void resize(float factor);

    // public:
    linkedHashTable(int numSlots = 10);
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
    ~linkedHashTable();
};