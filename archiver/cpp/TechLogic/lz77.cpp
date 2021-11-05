#include "lz77.h"
#include <fstream>

LZ77::LZ77(int window_size, int dictionary_size) {
    this->dictionary_size = dictionary_size * 1024;
    this->window_size = window_size * 1024;
    this->buffer_size = this->dictionary_size + this->window_size;
}

void LZ77::setDictionarySize(int _dictionary_size) {
    this->dictionary_size = _dictionary_size * 1024;
}

void LZ77::setWindowSize(int _window_size) {
    this->window_size = _window_size * 1024;
}

void LZ77::setBufferSize(int _buffer_size) {
    this->buffer_size = _buffer_size;
}

std::pair<vector<Code>, int> LZ77::getCodes(QString input) {
    std::ifstream fin;

    fin.open(input.toStdString(), std::ios::binary);

    vector<unsigned char> text(window_size + dictionary_size); //кольцевой буффер

    int index = 0, size_of_file = 0;

    unsigned char ch;
    //заполним буфер предпросмотра
    while (fin.is_open() && index < window_size && fin.read(reinterpret_cast<char*>(&ch), sizeof(char))) {
        text[index++] = ch;
        size_of_file++;
    }

    std::vector<Code> codes(0);

    int current_size_of_dict = 0;
    int start_of_window = 0;
    while (start_of_window < size_of_file) {
        Code c = getPrefix(text, start_of_window % buffer_size, current_size_of_dict);
        codes.push_back(c);

        start_of_window += c.length + 1;
        current_size_of_dict = min(start_of_window, dictionary_size);

        //теперь нужно считать c.length + 1 символ и сдвинуть указатели на окно и словарь
        int cnt = 0;
        while (fin.is_open() && cnt < c.length + 1 && fin.read(reinterpret_cast<char*>(&ch), sizeof(char))) {
            text[index] = ch;
            index = (index + 1) % buffer_size;
            cnt++;
            size_of_file++;
        }
    }

    fin.close();

    return {codes, size_of_file};
}

void LZ77::compressor(QString _compress_filename, QString _source_filename) {

    auto res = getCodes(_compress_filename);
    ofstream fout;
    fout.open(_source_filename.toStdString(), std::ios::binary);

    //////////////////////////
    QStringList path_list = _compress_filename.split(".");
    short file_extension_lenght = path_list.back().size();
    fout.write(reinterpret_cast<const char*>(&file_extension_lenght), sizeof(short));

    for (int i = 0; i < path_list.back().size(); i++) {
        fout.write(reinterpret_cast<const char*>(&path_list.back()[i]), sizeof(char));
    }
    //////////////////////////

    fout.write(reinterpret_cast<const char*>(&res.second), sizeof(int));

    for (int i = 0; i < (int)res.first.size(); ++i) {
        fout.write(reinterpret_cast<const char*>(&res.first[i].offset), sizeof(short));
        fout.write(reinterpret_cast<const char*>(&res.first[i].length), sizeof(short));
        fout.write(reinterpret_cast<const char*>(&res.first[i].next_char), sizeof(char));
    }
}

Code LZ77::getPrefix(vector<unsigned char> &s, int start_of_window, int current_size_of_dict) {
    int i = start_of_window - current_size_of_dict >= 0 ?
            start_of_window - current_size_of_dict :
            start_of_window - current_size_of_dict + buffer_size; // начало поиска в словаре
    int j = start_of_window; // поиск в буфере предпросмотра

    int offset = 0, length = 0;
    unsigned char new_char = s[start_of_window];

    for (int k = 0; k < current_size_of_dict; k++) {
        int i1 = (i + k) % buffer_size, j1 = j;

        int shift = 0;
        while (shift < window_size && s[i1] == s[j1]) {
            i1 = (i1 + 1) % buffer_size;
            j1 = (j1 + 1) % buffer_size;
            shift++;
        }

        if (shift > length) {
            length = shift;
            offset = current_size_of_dict - k;
            new_char = s[j1];
        }
    }
    return {(short)offset, (short)length, new_char};
}

void LZ77::decompressor(QString _source_filename, QString _decompress_filename) {
    vector<Code> codes(0);
    short offset, length;
    unsigned char new_char;

    ifstream fin;
    fin.open(_source_filename.toStdString(), ios::binary);

    ////////////////////////////
    short file_extension_lenght = 0;
    QString file_extension = "_lz.";
    fin.read(reinterpret_cast<char*>(&file_extension_lenght), sizeof(short));

    for (int i = 0; i < file_extension_lenght; i++) {
        char add_ext = 0;
        fin.read(reinterpret_cast<char*>(&add_ext), sizeof(char));
        file_extension.append(add_ext);
    }
    QString tmp_filename = _source_filename;
    _decompress_filename.append(tmp_filename.split("/").back().split(".").front().append(file_extension).prepend("/"));
    ///////////////////////////

    int size_of_file;
    fin.read(reinterpret_cast<char*>(&size_of_file), sizeof(int));

    while(fin.read(reinterpret_cast<char*>(&offset), sizeof(short))
            && fin.read(reinterpret_cast<char*>(&length), sizeof(short))
            && fin.read(reinterpret_cast<char*>(&new_char), sizeof(char))) {
        codes.push_back({offset, length, new_char});
    }

    vector<unsigned char > text(0);
    text.push_back(codes[0].next_char);
    int position = 1;
    for (int i = 1; i < (int)codes.size(); ++i) {
        for (int j = position - codes[i].offset; j < min(position - codes[i].offset + codes[i].length, (int)text.size()); ++j) {
            text.push_back(text[j]);
        }
        if ((int)text.size() < (int)size_of_file) {
            text.push_back(codes[i].next_char);
        }

        position = text.size();
    }

    ofstream fout;
    fout.open(_decompress_filename.toStdString(), ios::binary);
    for (int i = 0; i < (int)text.size(); ++i) {
        fout.write(reinterpret_cast<const char*>(&text[i]), sizeof(char));
    }
}

void LZ77::def_compressor(QString _compress_filename, QString _source_filename) {
    auto res = getCodes(_compress_filename);
    ofstream fout;
    fout.open(_source_filename.toStdString(), std::ios::binary);
    fout.write(reinterpret_cast<const char*>(&res.second), sizeof(int));

    for (int i = 0; i < (int)res.first.size(); ++i) {
        fout.write(reinterpret_cast<const char*>(&res.first[i].offset), sizeof(short));
        fout.write(reinterpret_cast<const char*>(&res.first[i].length), sizeof(short));
        fout.write(reinterpret_cast<const char*>(&res.first[i].next_char), sizeof(char));
    }
}

void LZ77::def_decompressor(QString _source_filename, QString _decompress_filename) {
    vector<Code> codes(0);
    short offset, length;
    unsigned char new_char;

    ifstream fin;
    fin.open(_source_filename.toStdString(), ios::binary);

    int size_of_file;
    fin.read(reinterpret_cast<char*>(&size_of_file), sizeof(int));

    while(fin.read(reinterpret_cast<char*>(&offset), sizeof(short))
            && fin.read(reinterpret_cast<char*>(&length), sizeof(short))
            && fin.read(reinterpret_cast<char*>(&new_char), sizeof(char))) {
        codes.push_back({offset, length, new_char});
    }

    vector<unsigned char > text(0);
    text.push_back(codes[0].next_char);
    int position = 1;
    for (int i = 1; i < (int)codes.size(); ++i) {
        for (int j = position - codes[i].offset; j < min(position - codes[i].offset + codes[i].length, (int)text.size()); ++j) {
            text.push_back(text[j]);
        }
        if ((int)text.size() < (int)size_of_file) {
            text.push_back(codes[i].next_char);
        }

        position = text.size();
    }

    ofstream fout;
    fout.open(_decompress_filename.toStdString(), ios::binary);
    for (int i = 0; i < (int)text.size(); i++) {
        fout.write(reinterpret_cast<const char*>(&text[i]), sizeof(char));
    }
    QFile(_source_filename).remove();
}



















