#include <assert.h>
#include <iostream>
#include <sstream>
using std::string, std::stringstream;

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
class rbtree {
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
        int compare(K other) {
            return key - other;
        }
        /**
         * @brief delete this node from the tree, fixing pointers accordingly
         *
         * @param[in] parentsChilda pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
         */
        void commitApoptosis(rbtree<K, V>::rbnode **parentsChild) {
            // if there are two children (hardest case)
            //  make this node's key the key of it's successor, then delete the successor node
            if (l && r) {
                rbtree<K, V>::rbnode *prev = this;
                rbtree<K, V>::rbnode *cur = r;
                while (cur->l) {
                    prev = cur;
                    cur = cur->l;
                }
                key = cur->key;
                if (prev->r == cur) {
                    // prev->r = nullptr;
                    cur->commitApoptosis(&(prev->r));
                } else {
                    // prev->l = nullptr;
                    cur->commitApoptosis(&(prev->l));
                }
                return;
            }
            // if there is a left or right child
            // replace this node with it, and fixup if need be
            else if (l) {
                *parentsChild = l;
                l->p = p;
                if (c == red) {
                    // nothing, all good
                } else if (l->c == red) {
                    l->c = black;
                } else if (c == black) {
                    doubleBlackFixup(l, l->p);
                }
            } else if (r) {
                *parentsChild = r;
                r->p = p;
                if (c == red) {
                    // nothing, all good
                } else if (r->c == red) {
                    r->c = black;
                } else if (c == black) {
                    doubleBlackFixup(r, r->p);
                }
            }
            // if it is a leaf, just fixup if need be
            else {
                *parentsChild = nullptr;
                if (c == black) {
                    doubleBlackFixup(NULL, p);
                }
            }
            // clear the pointers so it dosen't think it has children when it destructs
            l = nullptr;
            r = nullptr;
            // destruct
            delete this;
        }
        /**
         * @brief Get this node's uncle, if it exists
         *
         * @return rbtree<K, V>::rbnode* the uncle
         */
        rbtree<K, V>::rbnode *getUncle() {
            if (!p->p)
                return NULL;
            bool leftBranch = p == p->p->l;
            return leftBranch ? (p->p->r ? p->p->r : NULL) : (p->p->l ? p->p->l : NULL);
        }
        /**
         * @brief Get the insert case
         *
         * @return insertFixupInfo the fixup case info
         */
        insertFixupInfo getInsertCase() {
            insertFixupInfo info;
            if (!p) {
                info.fc = insertFixupInfo::root;
            } else if (!p->p || p->c == black) {
                info.fc = insertFixupInfo::good;
            } else if (getUncle() && getUncle()->c == red) {
                info.fc = insertFixupInfo::uncleRed;
                info.fo = (p == p->p->l) ? insertFixupInfo::right : insertFixupInfo::left;
            } else if (this == p->l && p == p->p->l) {
                info.fc = insertFixupInfo::line;
                info.fo = insertFixupInfo::left;
            } else if (this == p->r && p == p->p->l) {
                info.fc = insertFixupInfo::triangle;
                info.fo = insertFixupInfo::left;
            } else if (this == p->r && p == p->p->r) {
                info.fc = insertFixupInfo::line;
                info.fo = insertFixupInfo::right;
            } else if (this == p->l && p == p->p->r) {
                info.fc = insertFixupInfo::triangle;
                info.fo = insertFixupInfo::right;
            }
            return info;
        };
        /**
         * @brief the black magic to fix the red black tree after an insertion
         *
         */
        void insertFixup() {
            bool left;
            switch (getInsertCase().fc) {
            case insertFixupInfo::root:
                c = black;
                break;
            case insertFixupInfo::uncleRed:
                p->c = color(1 - p->c);
                p->p->c = color(1 - p->p->c);
                getUncle()->c = color(1 - getUncle()->c);
                p->p->insertFixup();
                break;
            case insertFixupInfo::line:
                left = (getInsertCase().fo == left);
                if (!left) {
                    p->p->rotateLeft();
                    p->c = black;
                    p->l->c = red;
                } else {
                    p->p->rotateRight();
                    p->c = black;
                    p->r->c = red;
                }
                break;
            case insertFixupInfo::triangle:
                left = (getInsertCase().fo == left);
                if (!left) {
                    p->rotateRight();
                    r->insertFixup();
                } else {
                    p->rotateLeft();
                    l->insertFixup();
                }
                break;
            default:
                break;
            }
        };
        /**
         * @brief perform a left rotation around this node
         */
        void rotateLeft() {
            rbtree<K, V>::rbnode *x = this;
            rbtree<K, V>::rbnode *y = r;
            r = y->l;
            if (y->l) {
                y->l->p = x;
            }
            y->p = x->p;
            if (!x->p) {
                setRoot(y);
            } else if (x == x->p->l) {
                x->p->l = y;
            } else {
                x->p->r = y;
            }
            y->l = x;
            x->p = y;
        }
        /**
         * @brief perform a right rotation around this node
         */
        void rotateRight() {
            rbtree<K, V>::rbnode *x = this;
            rbtree<K, V>::rbnode *y = l;
            l = y->r;
            if (y->r) {
                y->r->p = x;
            }
            y->p = x->p;
            if (!x->p) {
                setRoot(y);
            } else if (x == x->p->r) {
                x->p->r = y;
            } else {
                x->p->l = y;
            }
            y->r = x;
            x->p = y;
        }
        /**
         * @brief checks if a node is black (nil nodes are black)
         *
         * @param[in] n the node to check
         * @return true if it is black
         * @return false otherwise
         */
        static bool isBlack(rbtree<K, V>::rbnode *n) {
            return !n || n->c == black;
        }
        /**
         * @brief checks if a node is red (nil nodes are black)
         *
         * @param[in] n the node to check
         * @return true if it is red
         * @return false otherwise
         */
        static bool isRed(rbtree<K, V>::rbnode *n) {
            return n && n->c == red;
        }
        /**
         * @brief Get the dbnode's sibling, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling
         */
        static rbtree<K, V>::rbnode *getSibling(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
            if (!parent)
                return NULL;
            bool left = dbnode == parent->l;
            return left ? (parent->r ? parent->r : NULL) : (parent->l ? parent->l : NULL);
        }
        /**
         * @brief Get the dbnode's sibling's child furthest from the dbnode, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling's far child
         */
        static rbtree<K, V>::rbnode *getSiblingFarChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
            if (!parent)
                return NULL;
            bool left = dbnode == parent->l;
            return left ? (parent->r ? parent->r->r : NULL) : (parent->l ? parent->l->l : NULL);
        }
        /**
         * @brief Get the dbnode's sibling's child nearest to the dbnode, if it exists
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         * @return rbtree<K, V>::rbnode* the sibling's near child
         */
        static rbtree<K, V>::rbnode *getSiblingNearChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
            if (!parent)
                return NULL;
            bool left = dbnode == parent->l;
            return left ? (parent->r ? parent->r->l : NULL) : (parent->l ? parent->l->r : NULL);
        }
        /**
         * @brief the black magic to fix the red black tree after a removal
         * cases and how to fix them referenced from here: https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
         *
         * @param[in] dbnode the double black node that needs fixing
         * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
         */
        static void doubleBlackFixup(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
            if (!dbnode && !parent) {
                return;
            } else if (dbnode && !dbnode->p) {
                return;
            }
            rbtree<K, V>::rbnode *sib = getSibling(dbnode, parent);
            if (!sib) {
                std::cout << "⚠️⚠️⚠️ shouldn't be here ⚠️⚠️⚠️" << std::endl;
                assert(false);
                return;
            } else if (isBlack(sib) && isBlack(sib->l) && isBlack(sib->r)) {
                sib->c = red;
                if (isBlack(parent)) {
                    doubleBlackFixup(parent, parent->p);
                } else {
                    parent->c = black;
                }
            } else if (isRed(sib)) {
                color t = sib->c;
                sib->c = parent->c;
                parent->c = t;
                (parent->r == dbnode ? parent->rotateRight() : parent->rotateLeft());
                doubleBlackFixup(dbnode, parent);
            } else if (isBlack(sib) && isBlack(getSiblingFarChild(dbnode, parent)) && isRed(getSiblingNearChild(dbnode, parent))) {
                getSiblingNearChild(dbnode, parent)->c = black;
                sib->c = red;
                (sib == parent->r) ? sib->rotateRight() : sib->rotateLeft();
                doubleBlackFixup(dbnode, parent);
            } else if (isBlack(sib) && isRed(getSiblingFarChild(dbnode, parent))) {
                color t = sib->c;
                sib->c = parent->c;
                parent->c = t;
                getSiblingFarChild(dbnode, parent)->c = black;
                (parent->r == dbnode ? parent->rotateRight() : parent->rotateLeft());
            }
        }
        /**
         * @brief a string representation of this (singular) node
         *
         * @return string representation
         */
        string toString() {
            stringstream ss;
            switch (c) {
            case red:
                ss << "🟥";
                break;
            case black:
                ss << "⬛";
                break;
            }
            ss << "" << key << " -> " << value;
            return ss.str();
        }
        /**
         * @brief update the root reference of the tree object
         * @param[in] root a pointer to the new root
         */
        void setRoot(rbtree<K, V>::rbnode *root) {
            *(this->root) = root;
        }

    public:
        /**
         * @brief Construct a new rbnode object
         *
         * @param[in] key it's key
         * @param[in] value it's value
         * @param[in] parent it's parent
         * @param[in] r a pointer to the root node pointer, in case it needs to be updated
         */
        rbnode(K key, V value, rbtree<K, V>::rbnode *parent, rbtree<K, V>::rbnode **r) : key(key), root(r), p(parent), value(value) {
            if (!parent) {
                c = black;
            }
        };
        /**
         * @brief print the tree rooted at this node using a reverse pre order traversal
         *
         * @param[in] depth the depth of this node
         */
        void printTree(int depth) {
            std::cout << " ";
            for (int i = 0; i < depth; i++) {
                std::cout << "│ ";
            }
            std::cout << "\b" << toString() << std::endl;
            if (r) {
                r->printTree(depth + 1);
            }
            if (l)
                l->printTree(depth + 1);
        }
        /**
         * @brief makes a copy of the tree rooted at this node
         *
         * @param[in] parent the pointer to the parent of this copy
         * @return rbnode* the copy of the tree rooted here
         */
        rbnode *copy(rbnode *parent) {
            // make a copy of this node
            rbnode *newme = new rbnode(key, value, parent, root);
            // copy its subtrees, if they exist
            rbnode *newl = l ? l->copy(newme) : NULL;
            rbnode *newr = r ? r->copy(newme) : NULL;
            // update the pointers
            newme->l = newl;
            newme->r = newr;
            return newme;
        }
        /**
         * @brief check if the node with this key is in the tree rooted at this node
         *
         * @param[in] key the key to look for
         * @return true if the key is in this tree
         * @return false otherwise
         */
        bool contains(K key) {
            // if this is the node we are looking for, it is
            if (this->key == key) {
                return true;
            }
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree,
            // otherwise, see if the subtree contains it
            if (!*recurseOn) {
                return false;
            }
            return (*recurseOn)->contains(key);
        }
        /**
         * @brief get the value of the node with the key in the tree rooted at this node
         *
         * NOTE: this method has undefined behavior if the key is not in this subtree
         *
         * @param[in] key the key to look for
         * @return V the value of the key if it exists in the tree, undefined value otherwise
         */
        V getValue(K key) {
            // if this is the node we are looking for, get it
            if (this->key == key) {
                return value;
            }
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree,
            // otherwise, get the value of the node in the subtree
            if (!*recurseOn) {
                return (V)NULL;
            }
            return (*recurseOn)->getValue(key);
        }
        /**
         * @brief gets the key's value if it is in the tree rooted at this node
         *
         * @param[in] key the key to look for
         * @param[out] out the location to put the value if found
         * @return true if the key is in this tree
         * @return false otherwise
         */
        bool tryGetValue(K key, V *out) {
            // if this is the node we are looking for, get it
            if (this->key == key) {
                *out = value;
                return true;
            }
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree,
            // otherwise, get the value of the node in the subtree
            if (!*recurseOn) {
                return false;
            }
            return (*recurseOn)->tryGetValue(key, out);
        }
        /**
         * @brief set the value of a node with corrsponding key in the tree rooted at this node
         *
         * @param[in] key the key of the node to set
         * @param[in] value the value to set
         * @return true if the node was in the tree and set
         * @return false otherwise
         */
        bool setValue(K key, V value) {
            // if this is the node we are looking for, update it
            if (this->key == key) {
                this->value = value;
                return true;
            }
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree,
            // otherwise, set the value of the node in the subtree
            if (!*recurseOn) {
                return false;
            }
            return (*recurseOn)->setValue(key, value);
        }
        /**
         * @brief insert a key value pair into the tree rooted at this node
         *
         * @param[in] key the key of the node to insert
         * @param[in] value the value to be stored in the node
         * @return true if the node wasn't in the subtree and thus inserted
         * @return false otherwise
         */
        bool insert(K key, V value) {
            // if we are they key, it is already in the tree
            if (compare(key) == 0)
                return false;
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree, so insert it
            // otherwise, insert the node in the subtree
            if (!*recurseOn) {
                *recurseOn = new rbtree<K, V>::rbnode(key, value, this, root);
                (*recurseOn)->insertFixup();
                return true;
            } else {
                return (*recurseOn)->insert(key, value);
            }
        }
        /**
         * @brief remove a node with a key from the tree rooted at this node
         *
         * @param[in] key the key of the node to remove
         * @param[in] parentsChild a pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
         * @return true if the node was in the subtree and deleted
         * @return false otherwise
         */
        bool remove(K key, rbtree<K, V>::rbnode **parentsChild) {
            // if this is the node we are looking for, delete
            if (this->key == key) {
                commitApoptosis(parentsChild);
                return true;
            }
            // see if we need to look in the left or right subtree
            rbtree<K, V>::rbnode **recurseOn = (compare(key) >= 0) ? &l : &r;
            // if that child dosen't exist, the node isn't in the tree
            // otherwise, remove the node from the subtree
            if (!*recurseOn) {
                return false;
            } else {
                return (*recurseOn)->remove(key, recurseOn);
            }
        }
        /**
         * @brief get the black height of the tree rooted at this node
         *
         * @return int the bh if it is valid or -1 if the black height property is broken
         */
        int bh() {
            // if this is a leaf its bh is its color
            if (!l && !r) {
                return (c == red) ? 0 : 1;
            }
            // check left subtree, if it exists
            int lbh = l ? l->bh() : 0;
            // check right subtree, if it exists
            int rbh = r ? r->bh() : 0;
            // if one or both is broken, this is broken by default
            if (lbh == -1 || rbh == -1) {
                return -1;
            }
            // if they are equal, return their bh plus this node's color
            // else invalid
            return (lbh == rbh ? rbh + (c == red ? 0 : 1) : -1);
        }
        /**
         * @brief check to make sure the red property holds in the tree rooted at this node
         * true in the tree rooted at this node
         *
         * @return true if it does
         * @return false otherwise
         */
        bool rp() {
            // check left subtree, if it exists
            int lrp = l ? l->rp() : true;
            // check right subtree, if it exists
            int rrp = r ? r->rp() : true;
            // make sure this and its children don't make a new violation
            // then and everything together
            return (lrp && rrp && (c == red ? ((l ? l->c != red : true) && (r ? r->c != red : true)) : true));
        }
        /**
         * @brief Destroy the rbnode object and all its children recursively
         */
        ~rbnode() {
            if (r) {
                delete r;
            }
            if (l)
                delete l;
        }
    };
    /**
     * @brief a class returned by the subscripting operator []
     * that handles the getting or setting depending on what is done with it
     *
     * | something = getSetProxy: returns the value if it exists, undefined otherwise
     * | getSetProxyKeyInMap = value: sets the value in the map
     * | getSetProxyKeyNotInMap = value: inserts new key value pair into map
     *
     */
    class getSetProxy {
    private:
        /**
         * @brief theTree of which to perform the get/set operations
         */
        rbtree<K, V> *theTree;
        /**
         * @brief key of which to get/set
         */
        K key;

    public:
        /**
         * @brief need a reference to the tree to do the operations on and they key
         *
         * @param[in] theTree of which to perform the get/set operations
         * @param[in] key of which to get/set
         */
        getSetProxy(rbtree<K, V> *theTree, K key) : theTree(theTree), key(key) {}

        /**
         * @brief the cast operator, trying to cast to type value will get the correct value
         *
         * @return V the value corrsponding to key if it exists, undefined otherwise
         */
        operator V() const {
            return theTree->getValue(key);
        }
        /**
         * @brief overload the assignment operator to set the correct value in the map,
         * or insert it if it isn't there
         *
         * @param[in] value the value to set for the key
         */
        void operator=(V value) {
            if (!theTree->setValue(key, value)) {
                theTree->insert(key, value);
            }
        }
    };
    /**
     * @brief a pointer to the root node
     */
    rbtree<K, V>::rbnode *root = nullptr;

public:
    /**
     * @brief pretty print a representation of the rbtree
     */
    void printTree() {
        std::cout << "---------bh:" << (root ? root->bh() : 0) << "---------" << std::endl;
        if (!root) {
            return;
        }
        root->printTree(0);
        std::cout << "----------------------" << std::endl;
    }
    /**
     * @brief recursively deletes every node to clear the tree, this tree object still lives
     */
    void clear() {
        if (!root) {
            return;
        }
        delete root;
        root = NULL;
    }
    /**
     * @brief makes a new tree with the same contents as this one
     *
     * @return rbtree<K, V>*, a pointer to a new tree with the same contents
     */
    rbtree<K, V> *copy() {
        if (!root) {
            return new rbtree<K, V>();
        }
        rbtree<K, V> *ret = new rbtree<K, V>();
        ret->root = root->copy(NULL);
        return ret;
    }
    /**
     * @brief checks if the key is contained within the set/map
     *
     * @param[in] key the key to look for
     * @return true if the key is in the set/map
     * @return false otherwise
     */
    bool contains(K key) {
        if (!root) {
            return false;
        }
        return root->contains(key);
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
        if (!root) {
            return (V)NULL;
        }
        return root->getValue(key);
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
        if (!root) {
            return false;
        }
        return root->tryGetValue(key, out);
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
        if (!root) {
            return false;
        }
        return root->setValue(key, value);
    }
    /**
     * @brief insert a key/key-value pair in the map if it does not exist in the map
     *
     * @param[in] key the key to insert
     * @param[in] value [OPTIONAL] the value to insert (optional if only representing a set)
     * @return true if the key is not in the map/set and the value was inserted
     * @return false otherwise
     */
    bool insert(K key, V value = (V)NULL) {
        if (!root) {
            root = new rbtree<K, V>::rbnode(key, value, root, &root);
            return true;
        }
        bool ret = root->insert(key, value);
        assert(checkValid());
        return ret;
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
        return getSetProxy(this, key);
    }
    /**
     * @brief removes the key from the set/map if it exists
     *
     * @param[in] key the key to remove
     * @return true if the key was in the map and removed
     * @return false otherwise
     */
    bool remove(K key) {
        if (!root) {
            return false;
        }
        bool ret = root->remove(key, &root);
        assert(checkValid());
        return ret;
    }
    /**
     * @brief checks if the tree is valid in terms of black height property and red red property
     *
     * @return true of the tree is valid
     * @return false otherwise
     */
    bool checkValid() {
        if (!root) {
            return true;
        }
        return root->rp() && (root->bh() != -1);
    }
    /**
     * @brief clear all nodes and destroy the rbtree object
     */
    ~rbtree() {
        clear();
    }
};