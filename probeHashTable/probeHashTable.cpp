#include "./probeHashTable.h"
#include <assert.h>
#include <iostream>
#include <sstream>
using std::string, std::stringstream;

template <typename K, typename V>
unsigned int probeHashTable<K, V>::hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

template <typename K, typename V>
unsigned int probeHashTable<K, V>::polyProbe(unsigned int x) {
    return x;
}

template <typename K, typename V>
float probeHashTable<K, V>::loadFactor() {
    return (float)numKeys / (float)numSlots;
}

template <typename K, typename V>
void probeHashTable<K, V>::resize(float factor) {
    int prevSlots = numSlots;
    numSlots = numSlots * factor;
    numKeys = 0;
    probeHashTable<K, V>::kvPair *oldUnderlyingArray = underlyingArray;
    underlyingArray = new probeHashTable<K, V>::kvPair[(int)(numSlots)];
    for (int i = 0; i < prevSlots; i++) {
        probeHashTable<K, V>::kvPair *curr = &oldUnderlyingArray[i];
        if (!curr->empty) {
            insert(curr->key, curr->value);
        }
    }
    delete[] oldUnderlyingArray;
}

template <typename K, typename V>
probeHashTable<K, V>::probeHashTable(int numSlots) : numSlots(numSlots), numKeys(0), underlyingArray(new probeHashTable<K, V>::kvPair[numSlots]){};

template <typename K, typename V>
void probeHashTable<K, V>::print() {
    for (int i = 0; i < numSlots; i++) {
        std::cout << i << " : " << (underlyingArray[i].empty ? "EMPTY " : "") << underlyingArray[i].key << " -> " << underlyingArray[i].value << std::endl;
    }
}

template <typename K, typename V>
int probeHashTable<K, V>::getKeys(K *keys) {
    keys = (K *)malloc(numKeys * sizeof(K));
    int inPtr = 0;
    for (int i = 0; i < numSlots; i++) {
        if (underlyingArray[i].empty) {
            continue;
        }
        keys[inPtr++] = underlyingArray[i].key;
    }
    return numKeys;
};

template <typename K, typename V>
void probeHashTable<K, V>::clear() {
    for (int i = 0; i < numSlots; i++) {
        underlyingArray[i].empty = true;
    }
    numKeys = 0;
}

template <typename K, typename V>
IMap<K, V> *probeHashTable<K, V>::copy() {
    probeHashTable<K, V> *newTable = new probeHashTable<K, V>(numSlots);
    memcpy(newTable->underlyingArray, underlyingArray, numSlots * sizeof(probeHashTable));
    return newTable;
}

template <typename K, typename V>
bool probeHashTable<K, V>::contains(K key) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            return true;
        }
        // else if (curr->empty) {
        //     return false;
        // }
    }
    return false;
}

template <typename K, typename V>
V probeHashTable<K, V>::getValue(K key) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            return curr->value;
        }
        // else if (curr->empty) {
        //     return 0;
        // }
    }
    return 0;
}

template <typename K, typename V>
bool probeHashTable<K, V>::tryGetValue(K key, V *out) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            *out = curr->value;
            return true;
        }
        // else if (curr->empty) {
        //     return false;
        // }
    }
    return false;
}

template <typename K, typename V>
bool probeHashTable<K, V>::setValue(K key, V value) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            curr->value = value;
            return true;
        } else if (curr->empty) {
            return false;
        }
    }
    return false;
}

template <typename K, typename V>
bool probeHashTable<K, V>::insert(K key, V value) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            return false;
        } else if (curr->empty) {
            curr->key = key;
            curr->value = value;
            curr->empty = false;
            numKeys++;
            if (loadFactor() - alphaMax > 0) {
                resize(2);
            }
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
typename IMap<K, V>::getSetProxy probeHashTable<K, V>::operator[](K key) {
    return typename IMap<K, V>::getSetProxy(this, key);
}

template <typename K, typename V>
bool probeHashTable<K, V>::remove(K key) {
    int myHash = hash(key) % numSlots;
    for (int i = 0; i < numSlots; ++i) {
        int slot = (myHash + polyProbe(i)) % numSlots;
        probeHashTable<K, V>::kvPair *curr = &(underlyingArray[slot]);
        if (!curr->empty && curr->key == key) {
            curr->empty = true;
            numKeys--;
            if (loadFactor() - (alphaMax / 4) < 0) {
                resize(.5);
            }
            return true;
        }
        // else if (curr->empty) {
        //     return false;
        // }
    }
    return false;
}

template <typename K, typename V>
probeHashTable<K, V>::~probeHashTable() {
    clear();
}