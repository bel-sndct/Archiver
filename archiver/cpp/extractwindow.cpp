#include "extractwindow.h"
#include "ui_extractwindow.h"

ExtractWindow::ExtractWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractWindow) {

    this->ui->setupUi(this);
    this->ui->lineEdit_from->setReadOnly(true);
    this->ui->lineEdit_to->setReadOnly(true);
}

ExtractWindow::~ExtractWindow() {
    delete ui;
}

void ExtractWindow::setFilePathToExtLineEdit_from(QString _file_path) {
    this->absolute_arch_filepath = _file_path;
    this->filepath_to = _file_path;
    this->ui->lineEdit_from->setText(_file_path);
}

void ExtractWindow::setCurrentDirectory(QDir _dir_path) {
    this->directory = _dir_path;
    this->filepath_to = _dir_path.absolutePath();
    this->ui->lineEdit_to->setText(_dir_path.absolutePath());
}

void ExtractWindow::on_pushButton_cancel_clicked() {
    close();
}

void ExtractWindow::setDeflateProcessor(Node* _root) {
    this->deflate_processor.setDefRoot(_root);
}

void ExtractWindow::setHuffRoot(Node* _root) {
    this->huffman_processor.setRoot(_root);
}

void ExtractWindow::on_pushButton_change_path_clicked() {
    this->filepath_to = QFileDialog::getExistingDirectory(this, tr("Open directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->ui->lineEdit_to->setText(this->filepath_to);
}

void ExtractWindow::on_pushButton_confirm_clicked() {
    QStringList file_suffix = this->absolute_arch_filepath.split(".");
    if (this->ui->radioButton_lz77->isChecked() || this->ui->radioButton_huffman->isChecked()) {
        if (this->ui->lineEdit_to->text().size()) {

            if (this->ui->radioButton_lz77->isChecked()) {
                QString tmp_path = this->filepath_to;
                QString result_filename = tmp_path.append(this->absolute_arch_filepath.split("/").back().split(".").front().append("_lz.txt").prepend("/"));
                if (file_suffix.back() == "lz") {
                    this->lz77_processor.decompressor(this->absolute_arch_filepath, this->filepath_to);
                }
                else {
                    fileFormatExceptionHandler("Warning!", "The selected file is not suitable for processing by the LZ77 algorithm");
                }
            }
            else if (this->ui->radioButton_huffman->isChecked()) {
                QString tmp_path = this->filepath_to;
                QString result_filename = tmp_path.append(this->absolute_arch_filepath.split("/").back().split(".").front().append("_Huff.txt").prepend("/"));
                if (file_suffix.back() == "huff") {
                    this->huffman_processor.decompressor(this->absolute_arch_filepath, this->filepath_to);
                }
                else {
                    fileFormatExceptionHandler("Warning!", "The selected file is not suitable for processing by the Huffman algorithm");
                }
            }
            QMessageBox message(QMessageBox::Information, "Success", "The archive was extracted");
            message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
            message.exec();
        }
        else {
            fileFormatExceptionHandler("Warning!", "Choose where to save the file");
        }
    }
    else {
        fileFormatExceptionHandler("Warning!", "Choose archiving algorithm");
    }
}

void ExtractWindow::fileFormatExceptionHandler(QString _type, QString _message) {
    QMessageBox message(QMessageBox::Warning, _type, _message);
    message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    message.exec();
}
