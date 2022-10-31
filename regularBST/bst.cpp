#include <iostream>
enum color {
    red,
    black
};

template <typename T>
class bstnode {
private:
    T data;
    color c = red;
    bstnode<T> *l = nullptr;
    bstnode<T> *r = nullptr;
    bstnode<T> *p;

    int compare(T other) {
        return data - other;
    }
    void commitApoptosis(bstnode<T> **parentsChild) {
        if (l && r) {
            bstnode<T> *prev = this;
            bstnode<T> *cur = r;
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
    bstnode(T data, bstnode<T> *parent) {
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
        bstnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            return false;
        }
        return (*recurseOn)->contains(data);
    }
    bool insert(T data) {
        bstnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            *recurseOn = new bstnode(data, this);
            return true;
        } else {
            return (*recurseOn)->insert(data);
        }
    }
    bool remove(T data, bstnode<T> **parentsChild) {
        if (this->data == data) {
            commitApoptosis(parentsChild);
            return true;
        }
        bstnode<T> **recurseOn = (compare(data) >= 0) ? &l : &r;
        if (!*recurseOn) {
            return false;
        } else {
            return (*recurseOn)->remove(data, recurseOn);
        }
    }
};

template <typename T>
class bsttree {
private:
    bstnode<T> *root;

public:
    bsttree() {
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
            root = new bstnode<T>(data, nullptr);
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
    bsttree<int> *mybstt = new bsttree<int>();
    mybstt->insert(8);
    mybstt->insert(4);
    mybstt->insert(12);
    mybstt->insert(2);
    mybstt->insert(1);
    mybstt->insert(0);
    mybstt->insert(0);
    mybstt->insert(1);
    mybstt->insert(6);
    mybstt->insert(10);
    mybstt->insert(14);
    mybstt->remove(0);
    mybstt->remove(1);
    mybstt->remove(4);
    mybstt->remove(6);
    mybstt->preOrderTraverse();
    std::cout << mybstt->remove(8) << std::endl;
    mybstt->preOrderTraverse();
}