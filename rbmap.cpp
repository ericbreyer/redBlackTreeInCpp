#include "./rbtree.cpp"

/**
 * @brief wrapper over rbtree as a map
 *
 * @tparam K the typ of the key
 * @tparam V the type of the value
 */
template <typename K, typename V>
class rbmap {
private:
    rbtree<K, V> *underlying;
    /**
     * @brief construct a new rbmap object from an existing tree
     */
    rbmap(rbtree<K, V> *t) {
        underlying = t;
    }

public:
    /**
     * @brief Construct a new rbmap object
     */
    rbmap() {
        underlying = new rbtree<K, V>();
    }
    /**
     * @brief pretty print a representation of the rbtree
     */
    void print() {
        underlying->printTree();
    }
    /**
     * @brief recursively deletes every node to clear the tree, this tree object still lives
     */
    void clear() {
        underlying->clear();
    }
    /**
     * @brief makes a new tree with the same contents as this one
     *
     * @return rbtree<K, V>*, a pointer to a new tree with the same contents
     */
    rbmap<K, V> *copy() {
        return new rbmap<K, V>(underlying->copy());
    }
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    bool contains(K key) {
        return underlying->contains(key);
    };
    /**
     * @brief get the value of a key in the map.
     *
     * NOTE: this method has undefined behavior if the key is not in the map
     *
     * @param[in] key the key to look for
     * @return V the value of the key if it exists in the map, undefined value otherwise
     */
    V getValue(K key) {
        return underlying->getValue(key);
    };
    /**
     * @brief gets the key's value if it is in the map
     *
     * @param[in] key the key to look for
     * @param[out] out the location to put the value if found
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool tryGetValue(K key, V *out) {
        return underlying->tryGetValue(key, out);
    };
    /**
     * @brief Set the value of the key if it is in the map
     *
     * @param[in] key the key to find
     * @param[in] value the value to set
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool setValue(K key, V value) {
        return underlying->setValue(key, value);
    }
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    bool insert(K key, V value) {
        return underlying->insert(key, value);
    };
    /**
     * @brief get/set/insert operator with [], works like in python
     *
     * | tree[key]: returns the value if it exists, undefined otherwise
     * | tree[keyInMap] = value: sets the value in the map
     * | tree[keyNotInMap] = value: inserts new key value pair into map
     *
     * @param[in] key the key to get/set/insert
     * @return rbtree<K, V>::getSetProxy a proxy object that overloads
     * the right functions to make get/set/insert work smoothly
     */
    rbtree<K, V>::getSetProxy operator[](K key) {
        return underlying[key]
    }
    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    bool remove(K key) {
        return underlying->remove(key);
    }
    /**
     * @brief clear all nodes and destroy the rbtree object
     */
    ~rbmap() {
        delete underlying;
    };
};