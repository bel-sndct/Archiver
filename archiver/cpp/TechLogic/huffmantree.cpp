#include "huffmantree.h"

HuffmanTree::HuffmanTree() {
    this->root = nullptr;
}

void HuffmanTree::setRoot(Node* _root) {
    this->root = _root;
}

void HuffmanTree::readDataFromFile(QString& _filename, vector<char>& _data) {
    ifstream file(_filename.toStdString(), ios::binary | ios::ate);
    if (file.is_open()) {
        ifstream::pos_type position;
        position = file.tellg();
        _data.resize(position);
        file.seekg(0, ios::beg);
        file.read(&_data[0], position);
    }
    else {
        QMessageBox error_message(QMessageBox::Warning, "Error", "The file cannot be opened!");
        error_message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        error_message.exec();
    }
    file.close();
}

void HuffmanTree::compressor(QString& _compress_filename, QString& _source_filename) {
        vector<char> data;
        readDataFromFile(_compress_filename, data);

        map<char, int> symbols;
        for (auto i : data) { symbols[i]++; }

        list<Node*> tree;
        map<char, int>::iterator iter;

        for (iter = symbols.begin(); iter != symbols.end(); iter++) {
            Node* tmp_node = new Node(iter->first, iter->second);
            tree.push_back(tmp_node);
        }

        while(tree.size() != 1) {
            tree.sort(Comparator());

            Node* tmp_first = tree.front();
            tree.pop_front();
            Node* tmp_second = tree.front();
            tree.pop_front();

            Node* parent = new Node(tmp_first, tmp_second);
            tree.push_back(parent);
        }

        this->root = tree.front();
        tree.pop_front();

        ifstream file_codes(_compress_filename.toStdString());
        ofstream file_compressed(_source_filename.toStdString(), ios::out | ios::binary);

        /*//////////////////////////
        QStringList path_list = _compress_filename.split(".");
        short file_extension_lenght = path_list.back().size();
        file_compressed.write(reinterpret_cast<const char*>(&file_extension_lenght), sizeof(short));

        for (int i = 0; i < path_list.back().size(); i++) {
            file_compressed.write(reinterpret_cast<const char*>(&path_list.back()[i]), sizeof(char));
        }
        //////////////////////////*/

        createTable(this->root);

        bool flag = true;
        int counter = 0;
        char buffer = 0;

        char ch = file_codes.get();

        while(!file_codes.eof()) {
            char check_ch = file_codes.get();
            vector<bool> bit_sec = this->ciphers_table[ch];

            for (int i = 0; i < (int)bit_sec.size(); i++) {
                buffer = buffer | bit_sec[i] << (7 - counter);
                counter++;

                if (counter == 8 || (i == (int)bit_sec.size() - 1 && check_ch == EOF)) {
                    file_compressed << buffer;

                    if (check_ch == EOF) {
                        if (i != (int)bit_sec.size() - 1) {
                            counter = 0;
                            buffer = 0;
                            continue;
                        }
                        else {
                            char valid_bit_offset = counter;
                            file_compressed << valid_bit_offset;
                            flag = false;
                            break;
                        }
                    }
                    counter = 0;
                    buffer = 0;
                }
            }
            if (flag == false) break;
            ch = check_ch;
        }

        file_codes.close();
        file_compressed.close();
        data.clear();
        symbols.clear();
}

void HuffmanTree::decompressor(QString& _source_filename, QString& _decompress_filename) {

    ifstream file(_source_filename.toStdString(), ios::in | ios::binary);

        /*///////////////////////////
        short file_extension_lenght = 0;
        QString file_extension = "_huff.";
        file.read(reinterpret_cast<char*>(&file_extension_lenght), sizeof(short));

        for (int i = 0; i < file_extension_lenght; i++) {
            char add_ext = 0;
            file.read(reinterpret_cast<char*>(&add_ext), sizeof(char));
            file_extension.append(add_ext);
        }
        QString tmp_filename = _source_filename;
        _decompress_filename.append(tmp_filename.split("/").back().split(".").front().append(file_extension).prepend("/"));
        ///////////////////////////*/

        ofstream decompress_file(_decompress_filename.toStdString(), ios::out | ios::binary);

        Node* parent = this->root;
        int counter = 0;

        int valid_bit_offset = 0;
        char eof_check = 0;
        char byte = file.get();

        while (!file.eof()) {
            bool bt = byte &(1 << (7 - counter));

            if(bt) {
                parent = parent->getRightChild();
            }
            else {
                parent = parent->getLeftChild();
            }

            if (parent->getLeftChild() == nullptr && parent->getRightChild() == nullptr) {
                decompress_file << parent->getSymbol();
                parent = this->root;
            }
            counter++;

            if (counter == 8) {
                counter = 0;

                byte = file.get();
                valid_bit_offset = file.get();
                eof_check = file.get();

                if (valid_bit_offset >= 0 && valid_bit_offset <= 8 && eof_check == EOF) {
                    while (counter < valid_bit_offset) {
                        bool bt = byte &(1 << (7 - counter));

                        if(bt) {
                            parent = parent->getRightChild();
                        }
                        else {
                            parent = parent->getLeftChild();
                        }
                        if (parent->getLeftChild() == nullptr && parent->getRightChild() == nullptr) {
                            decompress_file << parent->getSymbol();
                            parent = this->root;
                        }
                        counter++;
                    }
                    if (counter == valid_bit_offset) break;
                }
                else {
                    file.unget();
                    file.unget();
                }
            }
        }

        file.close();
        decompress_file.close();
}

void HuffmanTree::createTable(Node* _root) {

    if (_root->getLeftChild()) {
        this->encrypted_sec.push_back(0);
        createTable(_root->getLeftChild());
    }
    if (_root->getRightChild()) {
        this->encrypted_sec.push_back(1);
        createTable(_root->getRightChild());
    }

    if (_root->getSymbol()) {
        this->ciphers_table[_root->getSymbol()] = this->encrypted_sec;
    }

    if (this->encrypted_sec.size()) {
        this->encrypted_sec.pop_back();
    }
}

void HuffmanTree::def_compressor(QString& _compress_filename, QString& _file_ext, QString& _source_filename) {
    vector<char> data;
    readDataFromFile(_compress_filename, data);

    map<char, int> symbols;
    for (auto i : data) { symbols[i]++; }

    list<Node*> tree;
    map<char, int>::iterator iter;

    for (iter = symbols.begin(); iter != symbols.end(); iter++) {
        Node* tmp_node = new Node(iter->first, iter->second);
        tree.push_back(tmp_node);
    }

    while(tree.size() != 1) {
        tree.sort(Comparator());

        Node* tmp_first = tree.front();
        tree.pop_front();
        Node* tmp_second = tree.front();
        tree.pop_front();

        Node* parent = new Node(tmp_first, tmp_second);
        tree.push_back(parent);
    }

    this->root = tree.front();
    tree.pop_front();

    ifstream file_codes;
    file_codes.open(_compress_filename.toStdString(), ios::binary);
    ofstream file_compressed;    //(_source_filename.toStdString(), ios::out | ios::binary);
    file_compressed.open(_source_filename.toStdString(), ios::binary);

    //////////////////////////
    QStringList path_list = _file_ext.split(".");
    short file_extension_lenght = path_list.back().size();
    file_compressed.write(reinterpret_cast<const char*>(&file_extension_lenght), sizeof(short));

    for (int i = 0; i < path_list.back().size(); i++) {
        file_compressed.write(reinterpret_cast<const char*>(&path_list.back()[i]), sizeof(char));
    }
    //////////////////////////

    createTable(this->root);

    bool flag = true;
    int counter = 0;
    char buffer = 0;

    char ch = 0;     //file_codes.get();
    file_codes.read(reinterpret_cast<char*>(&ch), sizeof(char));

    while(!file_codes.eof()) {
        char check_ch = 0;      //file_codes.get();
        file_codes.read(reinterpret_cast<char*>(&check_ch), sizeof(char));
        vector<bool> bit_sec = this->ciphers_table[ch];

        for (int i = 0; i < (int)bit_sec.size(); i++) {
            buffer = buffer | bit_sec[i] << (7 - counter);
            counter++;

            if (counter == 8 || (i == (int)bit_sec.size() - 1 && check_ch == EOF)) {
                //file_compressed << buffer;
                file_compressed.write(reinterpret_cast<const char*>(&buffer), sizeof(char));

                if (check_ch == EOF) {
                    if (i != (int)bit_sec.size() - 1) {
                        counter = 0;
                        buffer = 0;
                        continue;
                    }
                    else {
                        char valid_bit_offset = counter;
                        //file_compressed << valid_bit_offset;
                        file_compressed.write(reinterpret_cast<const char*>(&valid_bit_offset), sizeof(char));
                        flag = false;
                        break;
                    }
                }
                counter = 0;
                buffer = 0;
            }
        }
        if (flag == false) break;
        ch = check_ch;
    }

    file_codes.close();
    file_compressed.close();
    QFile(_compress_filename).remove();
    data.clear();
    symbols.clear();
}

void HuffmanTree::def_decompressor(QString& _source_filename, QString& _decompress_filename, QString& _res) {
    ifstream file;  //(_source_filename.toStdString(), ios::in | ios::binary);
    file.open(_source_filename.toStdString(), ios::binary);
    ofstream decompress_file;   //(_decompress_filename.toStdString());
    decompress_file.open(_decompress_filename.toStdString(), ios::binary);

    ////////////////////////////
    short file_extension_lenght = 0;
    QString file_extension = "_defl.";
    file.read(reinterpret_cast<char*>(&file_extension_lenght), sizeof(short));

    for (int i = 0; i < file_extension_lenght; i++) {
        char add_ext = 0;
        file.read(reinterpret_cast<char*>(&add_ext), sizeof(char));
        file_extension.append(add_ext);
    }
    QString tmp_filename = _source_filename;
    _res.append(tmp_filename.split("/").back().split(".").front().append(file_extension).prepend("/"));
    ///////////////////////////

    Node* parent = this->root;
    int counter = 0;

    int valid_bit_offset = 0;
    char eof_check = 0;
    char byte = 0;  //file.get();
    file.read(reinterpret_cast<char*>(&byte), sizeof(char));

    while (!file.eof()) {
        bool bt = byte &(1 << (7 - counter));

        if(bt) {
            parent = parent->getRightChild();
        }
        else {
            parent = parent->getLeftChild();
        }

        if (parent->getLeftChild() == nullptr && parent->getRightChild() == nullptr) {
            char send_char = parent->getSymbol();
            //decompress_file << parent->getSymbol();
            decompress_file.write(reinterpret_cast<const char*>(&send_char), sizeof(char));
            parent = this->root;
        }
        counter++;

        if (counter == 8) {
            counter = 0;

            byte = 0;   //file.get();
            file.read(reinterpret_cast<char*>(&byte), sizeof(char));
            ///////////////
            ios::pos_type ret_position = file.tellg();
            ///////////////
            valid_bit_offset = file.get();
            eof_check = 0;  //file.get();
            file.read(reinterpret_cast<char*>(&eof_check), sizeof(char));

            if (valid_bit_offset >= 0 && valid_bit_offset <= 8 && eof_check == EOF) {
                while (counter < valid_bit_offset) {
                    bool bt = byte &(1 << (7 - counter));

                    if(bt) {
                        parent = parent->getRightChild();
                    }
                    else {
                        parent = parent->getLeftChild();
                    }
                    if (parent->getLeftChild() == nullptr && parent->getRightChild() == nullptr) {
                        char send_char_n = parent->getSymbol();
                        //decompress_file << parent->getSymbol();
                        decompress_file.write(reinterpret_cast<const char*>(&send_char_n), sizeof(char));
                        parent = this->root;
                    }
                    counter++;
                }
                if (counter == valid_bit_offset) break;
            }
            else {
                //file.unget();
                //file.unget();
                file.seekg(ret_position);
            }
        }
    }

    file.close();
    //QFile(_source_filename).remove();
    decompress_file.close();
}





























