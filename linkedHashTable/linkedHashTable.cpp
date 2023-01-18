#include "./linkedHashTable.h"
#include <assert.h>
#include <iostream>
#include <sstream>
using std::string, std::stringstream;

template <typename K, typename V>
linkedHashTable<K, V>::chainingList::node::node(K key, V value) : key(key), value(value), next(nullptr) {}

template <typename K, typename V>
linkedHashTable<K, V>::chainingList::node::~node() {}

template <typename K, typename V>
linkedHashTable<K, V>::chainingList::chainingList() : root(nullptr) {
}

template <typename K, typename V>
bool linkedHashTable<K, V>::chainingList::insert(K key, V value) {
    node *prev = NULL;
    node *curr = root;
    while (curr != NULL) {
        if (key == curr->key) {
            return false;
        }
        prev = curr;
        curr = curr->next;
    }
    chainingList::node *newNode = new node(key, value);
    if (!root) { // root is null
        root = newNode;
    } else {
        prev->next = newNode;
    }
    return true;
}

template <typename K, typename V>
bool linkedHashTable<K, V>::chainingList::setValue(K key, V value) {
    node *prev = NULL;
    node *curr = root;
    while (curr != NULL) {
        if (key == curr->key) {
            curr->value = value;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

template <typename K, typename V>
bool linkedHashTable<K, V>::chainingList::remove(K key) {
    node *prev = NULL;
    node *curr = root;
    while (curr != NULL) {
        if (key == curr->key) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (prev && curr) { // key was found and is not root
        prev->next = curr->next;
        delete curr;
        curr = nullptr;
    } else if (!prev && curr) { // key was root
        root = nullptr;
        delete curr;
    } else { // key was not found
        return false;
    }
    return true;
}

template <typename K, typename V>
bool linkedHashTable<K, V>::chainingList::tryGet(K key, V *out) {
    node *curr = root;
    while (curr != NULL) {
        if (key == curr->key) {
            *out = curr->value;
            return true;
        }
        curr = curr->next;
    }
    return false;
}
template <typename K, typename V>
string linkedHashTable<K, V>::chainingList::toString() {
    stringstream ss;
    node *curr = root;
    while (curr != NULL) {
        ss << curr->key << " -> " << curr->value << " ==> ";
        curr = curr->next;
    }
    ss << "NIL";
    return ss.str();
};

template <typename K, typename V>
int linkedHashTable<K, V>::chainingList::keys(K *keys) {
    node *curr = root;
    int numKeys = 0;
    while (curr != NULL) {
        *(keys + numKeys) = curr->key;
        ++numKeys;
        curr = curr->next;
    }
    return numKeys;
}

template <typename K, typename V>
linkedHashTable<K, V>::chainingList::~chainingList() {
    node *prev = NULL;
    node *curr = root;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        delete prev;
    }
};

template <typename K, typename V>
unsigned int linkedHashTable<K, V>::hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
template <typename K, typename V>
float linkedHashTable<K, V>::loadFactor() {
    return (float)numKeys / (float)numSlots;
}

template <typename K, typename V>
void linkedHashTable<K, V>::resize(float factor) {
    if (numSlots <= 10 && factor < 1) {
        return;
    }
    int prevSlots = numSlots;
    numSlots = numSlots * factor;
    numKeys = 0;
    chainingList *oldUnderlyingArray = underlyingArray;
    underlyingArray = new chainingList[numSlots];
    for (int i = 0; i < prevSlots; i++) {
        chainingList *temp = oldUnderlyingArray + i;
        typename chainingList::node *curr = temp->root;

        while (curr != nullptr) {
            insert(curr->key, curr->value);
            curr = curr->next;
        }
    }
    // delete[] oldUnderlyingArray;
}

template <typename K, typename V>
linkedHashTable<K, V>::linkedHashTable(int numSlots) : numSlots(numSlots), numKeys(0), underlyingArray(new chainingList[numSlots]){};

template <typename K, typename V>
void linkedHashTable<K, V>::print() {
    for (int i = 0; i < numSlots; i++) {
        std::cout << i << " : " << underlyingArray[i].root << "    " << underlyingArray[i].toString() << std::endl;
    }
}

template <typename K, typename V>
int linkedHashTable<K, V>::getKeys(K *keys) {
    keys = (K *)malloc(numKeys * sizeof(K));
    int inPtr = 0;
    for (int i = 0; i < numSlots; i++) {
        inPtr += underlyingArray[i].keys(keys + inPtr);
    }
    return numKeys;
};

template <typename K, typename V>
void linkedHashTable<K, V>::clear() {
    for (int i = 0; i < numSlots; i++) {
        underlyingArray[i].~chainingList();
    }
    numKeys = 0;
}

template <typename K, typename V>
IMap<K, V> *linkedHashTable<K, V>::copy() {
    linkedHashTable<K, V> *newTable = new linkedHashTable<K, V>(numSlots);
    memcpy(newTable->underlyingArray, underlyingArray, numSlots * sizeof(linkedHashTable));
    return newTable;
}

template <typename K, typename V>
bool linkedHashTable<K, V>::contains(K key) {
    V temp;
    int slot = hash(key) % numSlots;
    return underlyingArray[slot].tryGet(key, &temp);
}

template <typename K, typename V>
V linkedHashTable<K, V>::getValue(K key) {
    V out;
    int slot = hash(key) % numSlots;
    underlyingArray[slot].tryGet(key, &out);
    return out;
}

template <typename K, typename V>
bool linkedHashTable<K, V>::tryGetValue(K key, V *out) {
    int slot = hash(key) % numSlots;
    return underlyingArray[slot].tryGet(key, out);
}

template <typename K, typename V>
bool linkedHashTable<K, V>::setValue(K key, V value) {
    int slot = hash(key) % numSlots;
    return underlyingArray[slot].setValue(key, value);
}

template <typename K, typename V>
bool linkedHashTable<K, V>::insert(K key, V value) {
    int slot = hash(key) % numSlots;
    bool ret = underlyingArray[slot].insert(key, value);
    if (ret) {
        numKeys++;
    }
    if (loadFactor() - alphaMax > 0) {
        resize(2);
    }
    return ret;
}

template <typename K, typename V>
typename IMap<K, V>::getSetProxy linkedHashTable<K, V>::operator[](K key) {
    return typename IMap<K, V>::getSetProxy(this, key);
}

template <typename K, typename V>
bool linkedHashTable<K, V>::remove(K key) {
    int slot = hash(key) % numSlots;
    bool ret = underlyingArray[slot].remove(key);
    if (ret) {
        numKeys--;
    }
    if (loadFactor() - (alphaMax / 4) < 0) {
        resize(.5);
    }
    return ret;
}

template <typename K, typename V>
linkedHashTable<K, V>::~linkedHashTable() {
    clear();
}