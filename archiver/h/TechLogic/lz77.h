#ifndef LZ77_H
#define LZ77_H

#include <iostream>
#include <qpair.h>
#include <vector>
#include <QString>
#include <QStringList>
#include <QFile>
using namespace std;

struct Code {
    short offset;
    short length;
    unsigned char next_char;
};

class LZ77 {
public:
    LZ77(int = 100, int = 80);
    ~LZ77() = default;

    void setDictionarySize(int);
    void setWindowSize(int);
    void setBufferSize(int);

    inline int getDictionarySize() { return this->dictionary_size; }
    inline int getWindowSize() { return this->window_size; }

    void compressor(QString, QString);
    void decompressor(QString, QString);

private:
    Code getPrefix(vector<unsigned char>&, int, int);
    pair<vector<Code>, int> getCodes(QString);

private:
    int dictionary_size;
    int window_size;
    int buffer_size;
};

#endif // LZ77_H
