#include <iostream>
enum color {
    red,
    black
};

template <typename T>
class rbnode {
private:
    T data;
    color c = red;
    rbnode<T> *l = nullptr;
    rbnode<T> *r = nullptr;
    rbnode<T> *p;

    int compare(T other) {
        return data - other;
    }
    void commitApoptosis(rbnode<T> **parentsChild) {
        if (l && r) {
            rbnode<T> *prev = this;
            rbnode<T> *cur = r;
            while (cur->l) {
                prev = cur;
                cur = cur->l;
            }
            *parentsChild = cur;

            if (prev != this) {
                prev->l = NULL;
            }
            cur->l = l;
            if (r != cur) {
                cur->r = r;
            } else {
                cur->r = NULL;
            }
        } else if (l) {
            *parentsChild = l;
        } else if (r) {
            *parentsChild = r;
        } else {
            *parentsChild = nullptr;
        }
        delete this;
    }

public:
    rbnode(T data, rbnode<T> *parent) {
        this->data = data;
        p = parent;
    };
    void preOrderTraverse(int depth) {
        for (int i = 0; i < depth; i++) {
            std::cout << "| ";
        }
        std::cout << "\b \b" << (depth ? "_" : "") << data << std::endl;
        if (l)
            l->preOrderTraverse(depth + 1);

        if (r)
            r->preOrderTraverse(depth + 1);
    }
    bool contains(T data) {
        if (this->data == data) {
            return true;
        }
        rbnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            return false;
        }
        return (*recurseOn)->contains(data);
    }
    bool insert(T data) {
        rbnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            *recurseOn = new rbnode(data, this);
            return true;
        } else {
            return (*recurseOn)->insert(data);
        }
    }
    bool remove(T data, rbnode<T> **parentsChild) {
        if (this->data == data) {
            commitApoptosis(parentsChild);
            return true;
        }
        rbnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            return false;
        } else {
            return (*recurseOn)->remove(data, recurseOn);
        }
    }
};

template <typename T>
class rbtree {
private:
    rbnode<T> *root;

public:
    rbtree() {
        root = nullptr;
    };
    void preOrderTraverse() {
        if (!root) {
            return;
        }
        root->preOrderTraverse(0);
    }
    bool contains(T data) {
        if (!root) {
            return false;
        }
        return root->contains(data);
    };
    void insert(T data) {
        if (!root) {
            root = new rbnode<T>(data, nullptr);
            return;
        }
        root->insert(data);
    };
    bool remove(T data) {
        if (!root) {
            return false;
        }
        return root->remove(data, &root);
    }
    void test() {
        std::cout << root->getData() << std::endl;
    };
};

int main() {
    rbtree<int> *myrbt = new rbtree<int>();
    myrbt->insert(8);
    myrbt->insert(4);
    myrbt->insert(12);
    myrbt->insert(2);
    myrbt->insert(1);
    myrbt->insert(0);
    myrbt->insert(0);
    myrbt->insert(1);
    myrbt->insert(6);
    myrbt->insert(10);
    myrbt->insert(14);
    myrbt->remove(0);
    myrbt->remove(1);
    myrbt->remove(4);
    myrbt->remove(6);
    myrbt->preOrderTraverse();
    std::cout << myrbt->remove(8) << std::endl;
    myrbt->preOrderTraverse();
}