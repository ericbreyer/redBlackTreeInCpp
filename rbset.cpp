#include "./rbtree.cpp"

/**
 * @brief wrapper over rbtree as a set
 *
 * @tparam T the parameter of the things stored in the set
 */
template <typename T>
class rbset {
private:
    rbtree<T> *underlying;
    /**
     * @brief construct a new rbset object from an existing tree
     */
    rbset(rbtree<T> *t) {
        underlying = t;
    }

public:
    /**
     * @brief Construct a new rbset object
     */
    rbset() {
        underlying = new rbtree<T>();
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
     * @return rbtree<T, V>*, a pointer to a new tree with the same contents
     */
    rbset<T> *copy() {
        return new rbset<T>(underlying->copy());
    }
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    bool contains(T key) {
        return underlying->contains(key);
    };
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    bool insert(T key) {
        return underlying->insert(key);
    };
    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    bool remove(T key) {
        return underlying->remove(key);
    }
    /**
     * @brief clear all nodes and destroy the rbtree object
     */
    ~rbset() {
        delete underlying;
    };
};