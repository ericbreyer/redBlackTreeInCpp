#include "./IMap.h"

template <typename K, typename V>
IMap<K, V>::getSetProxy::getSetProxy(IMap<K, V> *theMap, K key) : theMap(theMap), key(key){};
/**
 * @brief the cast operator, trying to cast to type value will get the correct value
 *
 * @return V the value corrsponding to key if it exists, undefined otherwise
 */
template <typename K, typename V>
IMap<K, V>::getSetProxy::operator V() const {
    return theMap->getValue(key);
};
/**
 * @brief overload the assignment operator to set the correct value in the map,
 * or insert it if it isn't there
 *
 * @param[in] value the value to set for the key
 */
template <typename K, typename V>
void IMap<K, V>::getSetProxy::operator=(V value) {
    if (!theMap->setValue(key, value)) {
        theMap->insert(key, value);
    }
};