#include "node.h"

Node::Node() {
    this->symbol = 0;
    this->height = 0;
    this->left_child = nullptr;
    this->right_child = nullptr;
}

Node::Node(char _symbol, int _count) {
    this->symbol = _symbol;
    this->height = _count;
    this->left_child = nullptr;
    this->right_child = nullptr;
}

Node::Node(Node* _left, Node* _right) {
    this->left_child = _left;
    this->right_child = _right;
    this->height = this->left_child->height
                 + this->right_child->height;
    this->symbol = 0;
}

Node::~Node() {
    if (this->left_child != nullptr) {
        delete this->left_child;
    }
    if (this->right_child != nullptr) {
        delete this->right_child;
    }

    this->symbol = 0;
    this->height = 0;
    this->left_child = nullptr;
    this->right_child = nullptr;
}

char Node::getSymbol() {
    return this->symbol;
}

Node* Node::getLeftChild() {
    return this->left_child;
}

Node* Node::getRightChild() {
    return this->right_child;
}

int Node::getHeight() const {
    return this->height;
}

void Node::setHeight(int _count) {
    this->height = _count;
}

void Node::setChildNodes(Node* _left, Node* _right) {
    this->left_child = _left;
    this->right_child = _right;
}












