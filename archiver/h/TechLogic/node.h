#ifndef NODE_H
#define NODE_H

class Node {
public:
    Node();
    Node(int, int);
    Node(char, int);
    Node(Node*, Node*);
    ~Node();

    void setHeight(int);
    void setChildNodes(Node*, Node*);

    char getSymbol();
    Node* getLeftChild();
    Node* getRightChild();
    int getHeight() const;

private:
    Node* left_child;
    Node* right_child;
    char symbol;
    int height;
};

#endif // NODE_H
