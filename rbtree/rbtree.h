#include "../IMap/IMap.h"
#include <string>
#pragma once
#define STRICT_CHECK
#ifndef STRICT_CHECK
#define assert void
#endif

/**
 * @brief a red black tree implementation of a set/map
 *
 * @tparam K the type of the key, must be totally orderable using operator-
 * @tparam V the value paired with the key, default is int (if just using a set doesn't need to be specified)
 */
template <typename K, typename V = int>
class rbtree : public IMap<K, V> {
private:
    /**
     * @brief enum of the color of the node
     */
    enum color {
        red,
        black
    };
    /**
     * @brief a composition of enums that give case and direction of insert fixup case
     */
    struct insertFixupInfo {
    public:
        enum insertFixupCase {
            root,
            uncleRed,
            triangle,
            line,
            good
        };
        enum insertFixupOrientation {
            left,
            right
        };
        insertFixupCase fc;
        insertFixupOrientation fo;
    };

    /**
     * @brief a class that represents a rbtree node
     */
    class rbnode {
    private:
        /**
         * @brief a pointer to the pointer of the tree object's root
         */
        rbtree<K, V>::rbnode **root = nullptr;
        /**
         * @brief this nodes color
         */
        color c = red;
        /**
         * @brief the left child of this node
         */
        rbtree<K, V>::rbnode *l = nullptr;
        /**
         * @brief the right child of this node
         */
        rbtree<K, V>::rbnode *r = nullptr;
        /**
         * @brief the parent of this node
         */
        rbtree<K, V>::rbnode *p = nullptr;
        /**
         * @brief the key of this node
         */
        K key;
        /**
         * @brief the value stored in this node
         */
        V value;

        /**
         * @brief provides a total ordering of keys.
         * K must overload operator-, ie. of a numeric type or IComparable
         *
         * @param[in] other an object of type K to compare
         * @return int 0 if equal, positive if this is greater, negative if other is greater
         */
        int compare(K other);
        /**
         * @brief delete this node from the tree, fixing pointers accordingly
         *
         * @param[in] parentsChilda pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
         */
        void commitApoptosis(rbtree<K, V>::rbnode **parentsChild);
        /**
         * @brief Get this node's uncle, if it exists
         *
         * @return rbtree<K, V>::rbnode* the uncle
         */
        rbtree<K, V>::rbnode *getUncle();
        /**
         * @brief Get the insert case
         *
         * @return insertFixupInfo the fixup case info
         */
        insertFixupInfo getInsertCase();
        /**
         * @brief the black magic to fix the red black tree after an insertion
         *
         */
        void insertFixup();
        /**
         * @brief perform a left rotation around this node
         */
        void rotateLeft();
        /**
         * @brief perform a right rotation around this node
         */
        void rotateRight();
        /**
         * @brief checks if a node is black (nil nodes are black)
         *
         * @param[in] n the node to check
         * @return true if it is black
         * @return false otherwise
         */
        static bool isBlack(rbtree<K, V>::rbnode *n);
        /**
         * @brief checks if a node is red (nil nodes are black)
         *
         * @param[in] n the node to check
         * @return true if it is red
         * @return false otherwise
         */
        static bool isRed(rbtree<K, V>::rbnode *n);
        /**
         * @brief Get the dbnode's sibling, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling
         */
        static rbtree<K, V>::rbnode *getSibling(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent);
        /**
         * @brief Get the dbnode's sibling's child furthest from the dbnode, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling's far child
         */
        static rbtree<K, V>::rbnode *getSiblingFarChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent);
        /**
         * @brief Get the dbnode's sibling's child nearest to the dbnode, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling's near child
         */
        static rbtree<K, V>::rbnode *getSiblingNearChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent);
        /**
         * @brief the black magic to fix the red black tree after a removal
         * cases and how to fix them referenced from here: https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         */
        static void doubleBlackFixup(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent);
        /**
         * @brief a string representation of this (singular) node
         *
         * @return string representation
         */
        std::string toString();
        /**
         * @brief update the root reference of the tree object
         * @param[in] root a pointer to the new root
         */
        void setRoot(rbtree<K, V>::rbnode *root);

    public:
        /**
         * @brief Construct a new rbnode object
         *
         * @param[in] key it's key
         * @param[in] value it's value
         * @param[in] parent it's parent
         * @param[in] r a pointer to the root node pointer, in case it needs to be updated
         */
        rbnode(K key, V value, rbtree<K, V>::rbnode *parent, rbtree<K, V>::rbnode **r);
        /**
         * @brief print the tree rooted at this node using a reverse pre order traversal
         *
         * @param[in] depth the depth of this node
         */
        void printTree(int depth);

        int getKeys(K *keys);
        /**
         * @brief makes a copy of the tree rooted at this node
         *
         * @param[in] parent the pointer to the parent of this copy
         * @return rbnode* the copy of the tree rooted here
         */
        rbnode *copy(rbnode *parent);
        /**
         * @brief check if the node with this key is in the tree rooted at this node
         *
         * @param[in] key the key to look for
         * @return true if the key is in this tree
         * @return false otherwise
         */
        bool contains(K key);
        /**
         * @brief get the value of the node with the key in the tree rooted at this node
         *
         * NOTE: this method has undefined behavior if the key is not in this subtree
         *
         * @param[in] key the key to look for
         * @return V the value of the key if it exists in the tree, undefined value otherwise
         */
        V getValue(K key);
        /**
         * @brief gets the key's value if it is in the tree rooted at this node
         *
         * @param[in] key the key to look for
         * @param[out] out the location to put the value if found
         * @return true if the key is in this tree
         * @return false otherwise
         */
        bool tryGetValue(K key, V *out);
        /**
         * @brief set the value of a node with corrsponding key in the tree rooted at this node
         *
         * @param[in] key the key of the node to set
         * @param[in] value the value to set
         * @return true if the node was in the tree and set
         * @return false otherwise
         */
        bool setValue(K key, V value);
        /**
         * @brief insert a key value pair into the tree rooted at this node
         *
         * @param[in] key the key of the node to insert
         * @param[in] value the value to be stored in the node
         * @return true if the node wasn't in the subtree and thus inserted
         * @return false otherwise
         */
        bool insert(K key, V value);
        /**
         * @brief remove a node with a key from the tree rooted at this node
         *
         * @param[in] key the key of the node to remove
         * @param[in] parentsChild a pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
         * @return true if the node was in the subtree and deleted
         * @return false otherwise
         */
        bool remove(K key, rbtree<K, V>::rbnode **parentsChild);
        /**
         * @brief get the black height of the tree rooted at this node
         *
         * @return int the bh if it is valid or -1 if the black height property is broken
         */
        int bh();
        /**
         * @brief check to make sure the red property holds in the tree rooted at this node
         * true in the tree rooted at this node
         *
         * @return true if it does
         * @return false otherwise
         */
        bool rp();
        /**
         * @brief Destroy the rbnode object and all its children recursively
         */
        ~rbnode();
    };
    /**
     * @brief a pointer to the root node
     */
    rbtree<K, V>::rbnode *root = nullptr;

public:
    /**
     * @brief pretty print a representation of the rbtree
     */
    void print();

    int getKeys(K *keys);

    /**
     * @brief recursively deletes every node to clear the tree, this tree object still lives
     */
    void clear();
    /**
     * @brief makes a new tree with the same contents as this one
     *
     * @return rbtree<K, V>*, a pointer to a new tree with the same contents
     */
    rbtree<K, V> *copy();
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    bool contains(K key);
    /**
     * @brief get the value of a key in the map.
     *
     * NOTE: this method has undefined behavior if the key is not in the map
     *
     * @param[in] key the key to look for
     * @return V the value of the key if it exists in the map, undefined value otherwise
     */
    V getValue(K key);
    /**
     * @brief gets the key's value if it is in the map
     *
     * @param[in] key the key to look for
     * @param[out] out the location to put the value if found
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool tryGetValue(K key, V *out);
    /**
     * @brief Set the value of the key if it is in the map
     *
     * @param[in] key the key to find
     * @param[in] value the value to set
     * @return true if the key is in the map
     * @return false otherwise
     */
    bool setValue(K key, V value);
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    bool insert(K key, V value = (V)NULL);
    /**
     * @brief get/set/insert operator with [], works like in python
     *
     * | tree[key]: returns the value if it exists, undefined otherwise
     * | tree[keyInMap] = value: sets the value in the map
     * | tree[keyNotInMap] = value: inserts new key value pair into map
     *
     * @param[in] key the key to get/set/insert
     * @return getSetProxy a proxy object that overloads
     * the right functions to make get/set/insert work smoothly
     */
    typename IMap<K, V>::getSetProxy operator[](K key);
    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    bool remove(K key);
    /**
     * @brief checks if the tree is valid in terms of black height property and red red property
     *
     * @return true of the tree is valid
     * @return false otherwise
     */
    bool checkValid();
    /**
     * @brief clear all nodes and destroy the rbtree object
     */
    ~rbtree();
};