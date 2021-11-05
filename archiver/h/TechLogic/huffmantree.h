#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <map>
#include <vector>
#include <QString>
#include <fstream>
#include <QMessageBox>
#include <QFile>
#include "node.h"
using namespace std;

class HuffmanTree {
public:
    HuffmanTree();
    ~HuffmanTree() = default;

    inline Node* getRoot() { return this->root; }

    void setRoot(Node*);
    void compressor(QString&, QString&);
    void decompressor(QString&, QString&);

private:
    void readDataFromFile(QString&, vector<char>&);
    void createTable(Node*);

private:
    Node* root;
    vector<bool> encrypted_sec;
    map<char, vector<bool>> ciphers_table;
};

struct Comparator {
    bool operator()(const Node* _left, const Node* _right) {
        return (_left->getHeight() < _right->getHeight());
    }
};

#endif // HUFFMANTREE_H
