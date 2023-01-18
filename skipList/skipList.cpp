class skipList {
private:
    class node {
    private:
        int level;
        node (*next)[];
        int gap[];

    public:
        node(int level);
        ~node();j
    };

public:
    skipList(/* args */);
    ~skipList();
};