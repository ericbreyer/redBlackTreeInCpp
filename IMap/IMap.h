#pragma once

#include <typeinfo>

template <typename K, typename V = int>
class IMap {
private:
    IMap(IMap<K, V> &other);

public:
    class getSetProxy {
    private:
        /**
         * @brief theMap of which to perform the get/set operations
         */
        IMap<K, V> *theMap;
        /**
         * @brief key of which to get/set
         */
        K key;

    public:
        getSetProxy(IMap<K, V> *theMap, K key);
        operator V() const;
        void operator=(V value);
    };
    IMap(){};
    virtual void print() = 0;
    virtual void clear() = 0;
    virtual IMap<K, V> *copy() = 0;
    virtual bool contains(K key) = 0;
    virtual V getValue(K key) = 0;
    virtual bool tryGetValue(K key, V *out) = 0;
    virtual bool setValue(K key, V value) = 0;
    virtual bool insert(K key, V value = 0) = 0;
    virtual typename IMap<K, V>::getSetProxy operator[](K key) = 0;
    virtual bool remove(K key) = 0;
    virtual int getKeys(K *keys) = 0;
    virtual ~IMap(){};
};