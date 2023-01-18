#include "./rbtree.h"
#include <assert.h>
#include <iostream>
#include <sstream>

using std::string, std::stringstream;

template <typename K, typename V>
int rbtree<K, V>::rbnode::compare(K other) {
    return key - other;
}

template <typename K, typename V>
void rbtree<K, V>::rbnode::commitApoptosis(rbtree<K, V>::rbnode **parentsChild) {
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

template <typename K, typename V>
typename rbtree<K, V>::rbnode *rbtree<K, V>::rbnode::getUncle() {
    if (!p->p)
        return NULL;
    bool leftBranch = p == p->p->l;
    return leftBranch ? (p->p->r ? p->p->r : NULL) : (p->p->l ? p->p->l : NULL);
}

template <typename K, typename V>
typename rbtree<K, V>::insertFixupInfo rbtree<K, V>::rbnode::getInsertCase() {
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

template <typename K, typename V>
void rbtree<K, V>::rbnode::insertFixup() {
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

template <typename K, typename V>
void rbtree<K, V>::rbnode::rotateLeft() {
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

template <typename K, typename V>
void rbtree<K, V>::rbnode::rotateRight() {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::isBlack(rbtree<K, V>::rbnode *n) {
    return !n || n->c == black;
}

template <typename K, typename V>
bool rbtree<K, V>::rbnode::isRed(rbtree<K, V>::rbnode *n) {
    return n && n->c == red;
}

template <typename K, typename V>
typename rbtree<K, V>::rbnode *rbtree<K, V>::rbnode::getSibling(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
    if (!parent)
        return NULL;
    bool left = dbnode == parent->l;
    return left ? (parent->r ? parent->r : NULL) : (parent->l ? parent->l : NULL);
}

template <typename K, typename V>
typename rbtree<K, V>::rbnode *rbtree<K, V>::rbnode::getSiblingFarChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
    if (!parent)
        return NULL;
    bool left = dbnode == parent->l;
    return left ? (parent->r ? parent->r->r : NULL) : (parent->l ? parent->l->l : NULL);
}

template <typename K, typename V>
typename rbtree<K, V>::rbnode *rbtree<K, V>::rbnode::getSiblingNearChild(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
    if (!parent)
        return NULL;
    bool left = dbnode == parent->l;
    return left ? (parent->r ? parent->r->l : NULL) : (parent->l ? parent->l->r : NULL);
}

template <typename K, typename V>
void rbtree<K, V>::rbnode::doubleBlackFixup(rbtree<K, V>::rbnode *dbnode, rbtree<K, V>::rbnode *parent) {
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

template <typename K, typename V>
string rbtree<K, V>::rbnode::toString() {
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

template <typename K, typename V>
void rbtree<K, V>::rbnode::setRoot(rbtree<K, V>::rbnode *root) {
    *(this->root) = root;
}

template <typename K, typename V>
rbtree<K, V>::rbnode::rbnode(K key, V value, rbtree<K, V>::rbnode *parent, rbtree<K, V>::rbnode **r) : key(key), root(r), p(parent), value(value) {
    if (!parent) {
        c = black;
    }
};

template <typename K, typename V>
void rbtree<K, V>::rbnode::printTree(int depth) {
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

template <typename K, typename V>
int rbtree<K, V>::rbnode::getKeys(K *keys) {

    int numRKeys = 0;
    int numLKeys = 0;
    K *rkeys = NULL;
    K *lkeys = NULL;

    if (r) {
        numRKeys = r->getKeys(rkeys);
    }
    if (l) {
        numLKeys = l->getKeys(lkeys);
    }

    keys = (K *)malloc(sizeof(K) * numRKeys + numLKeys + 1);
    *keys = this->key;
    if (rkeys) {
        memcpy(keys + 1, rkeys, numRKeys);
    }
    if (lkeys) {
        memcpy(keys + 1 + numRKeys, lkeys, numLKeys);
    }
    delete rkeys;
    delete lkeys;
    return numRKeys + numLKeys + 1;
}

template <typename K, typename V>
typename rbtree<K, V>::rbnode *rbtree<K, V>::rbnode::copy(rbnode *parent) {
    // make a copy of this node
    rbnode *newme = new rbnode(key, value, parent, root);
    newme->c = c;
    // copy its subtrees, if they exist
    rbnode *newl = l ? l->copy(newme) : NULL;
    rbnode *newr = r ? r->copy(newme) : NULL;
    // update the pointers
    newme->l = newl;
    newme->r = newr;
    return newme;
}

template <typename K, typename V>
bool rbtree<K, V>::rbnode::contains(K key) {
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

template <typename K, typename V>
V rbtree<K, V>::rbnode::getValue(K key) {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::tryGetValue(K key, V *out) {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::setValue(K key, V value) {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::insert(K key, V value) {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::remove(K key, rbtree<K, V>::rbnode **parentsChild) {
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

template <typename K, typename V>
int rbtree<K, V>::rbnode::bh() {
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

template <typename K, typename V>
bool rbtree<K, V>::rbnode::rp() {
    // check left subtree, if it exists
    int lrp = l ? l->rp() : true;
    // check right subtree, if it exists
    int rrp = r ? r->rp() : true;
    // make sure this and its children don't make a new violation
    // then and everything together
    return (lrp && rrp && (c == red ? ((l ? l->c != red : true) && (r ? r->c != red : true)) : true));
}

template <typename K, typename V>
rbtree<K, V>::rbnode::~rbnode() {
    if (r) {
        delete r;
    }
    if (l)
        delete l;
}

template <typename K, typename V>
void rbtree<K, V>::print() {
    std::cout << "---------bh:" << (root ? root->bh() : 0) << "---------" << std::endl;
    if (!root) {
        return;
    }
    root->printTree(0);
    std::cout << "----------------------" << std::endl;
}

template <typename K, typename V>
int rbtree<K, V>::getKeys(K *keys) {
    if (!root) {
        return 0;
    }
    return root->getKeys(keys);
}

template <typename K, typename V>
void rbtree<K, V>::clear() {
    if (!root) {
        return;
    }
    delete root;
    root = NULL;
}

template <typename K, typename V>
rbtree<K, V> *rbtree<K, V>::copy() {
    if (!root) {
        return new rbtree<K, V>();
    }
    rbtree<K, V> *ret = new rbtree<K, V>();
    ret->root = root->copy(NULL);
    return ret;
}

template <typename K, typename V>
bool rbtree<K, V>::contains(K key) {
    if (!root) {
        return false;
    }
    return root->contains(key);
};

template <typename K, typename V>
V rbtree<K, V>::getValue(K key) {
    if (!root) {
        return (V)NULL;
    }
    return root->getValue(key);
};

template <typename K, typename V>
bool rbtree<K, V>::tryGetValue(K key, V *out) {
    if (!root) {
        return false;
    }
    return root->tryGetValue(key, out);
};

template <typename K, typename V>
bool rbtree<K, V>::setValue(K key, V value) {
    if (!root) {
        return false;
    }
    return root->setValue(key, value);
}

template <typename K, typename V>
bool rbtree<K, V>::insert(K key, V value) {
    if (!root) {
        root = new rbtree<K, V>::rbnode(key, value, root, &root);
        return true;
    }
    bool ret = root->insert(key, value);
    assert(checkValid());
    return ret;
};

template <typename K, typename V>
typename IMap<K, V>::getSetProxy rbtree<K, V>::operator[](K key) {
    return typename IMap<K, V>::getSetProxy((IMap<K, V> *)this, key);
}

template <typename K, typename V>
bool rbtree<K, V>::remove(K key) {
    if (!root) {
        return false;
    }
    bool ret = root->remove(key, &root);
    assert(checkValid());
    return ret;
}

template <typename K, typename V>
bool rbtree<K, V>::checkValid() {
    if (!root) {
        return true;
    }
    return root->rp() && (root->bh() != -1);
}

template <typename K, typename V>
rbtree<K, V>::~rbtree() {
    clear();
}