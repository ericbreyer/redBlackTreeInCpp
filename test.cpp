#include "./IComparable.cpp"
#include "./rbtree.cpp"
#include <assert.h>
class funnyInt : public IComparable {
private:
    int data;

public:
    funnyInt(int v) {
        data = v;
    };
    int getIntRep() override {
        return data;
    }
    int operator-(IComparable *rhs) override {
        return data - rhs->getIntRep();
    };
    bool operator==(IComparable *rhs) override {
        return data - rhs->getIntRep() == 0;
    };
};

int main() {
    rbtree<int, float> mytree = rbtree<int, float>();
    mytree.insert(1, 3.5);
    mytree.insert(2);
    mytree.insert(3);
    mytree.insert(4);
    mytree.insert(5);
    mytree.insert(6);
    mytree.insert(7, 5);
    mytree.insert(8, 8);
    mytree.insert(9);
    mytree.insert(10);
    mytree.remove(4);
    mytree.insert(11);
    mytree.insert(12);
    mytree.insert(13);
    mytree.insert(14);
    mytree.insert(15);
    mytree.insert(16);
    mytree[7] = 8;
    float t = mytree[7];
    std::cout << t << std::endl;
    mytree.printTree();
}