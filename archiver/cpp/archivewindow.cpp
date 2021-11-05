#include "archivewindow.h"
#include "ui_archivewindow.h"

ArchiveWindow::ArchiveWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ArchiveWindow) {

    ui->setupUi(this);
    this->ui->lineEdit_from->setReadOnly(true);
    this->ui->lineEdit_to->setReadOnly(true);
}

ArchiveWindow::~ArchiveWindow() {
    delete ui;
}

void ArchiveWindow::setDeflateProcessor(Node* _root) {
    this->deflate_processor.setDefRoot(_root);
}

void ArchiveWindow::setFilePathToArchLineEdit_from(QString _file_path) {
    this->absolute_arch_filepath = _file_path;
    this->ui->lineEdit_from->setText(_file_path);
}

void ArchiveWindow::setCurrentDirectory(QDir _dir_path) {
    this->directory = _dir_path;
    this->filepath_to = _dir_path.absolutePath();
    this->ui->lineEdit_to->setText(_dir_path.absolutePath());
}

void ArchiveWindow::on_pushButton_cancel_clicked() {
    close();
}

void ArchiveWindow::setHuffRoot(Node* _root) {
    this->huffman_processor.setRoot(_root);
}

void ArchiveWindow::on_pushButton_change_path_clicked() {
    this->filepath_to = QFileDialog::getExistingDirectory(this, tr("Open directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    this->ui->lineEdit_to->setText(this->filepath_to);
}

void ArchiveWindow::on_pushButton_confirm_clicked() {
    if (this->ui->radioButton_lz77->isChecked() || this->ui->radioButton_huffman->isChecked()) {
        if (this->ui->lineEdit_to->text().size()) {
            QString compress_ratio = "The archive was created";

            if (this->ui->radioButton_lz77->isChecked()) {
                QString tmp_path = this->filepath_to;
                QString result_filename = tmp_path.append(this->absolute_arch_filepath.split("/").back().split(".").front().append(".lz").prepend("/"));
                this->lz77_processor.compressor(this->absolute_arch_filepath, result_filename);
            }
            else if (this->ui->radioButton_huffman->isChecked()) {
                QString tmp_path = this->filepath_to;
                QString result_filename = tmp_path.append(this->absolute_arch_filepath.split("/").back().split(".").front().append(".huff").prepend("/"));
                this->huffman_processor.compressor(this->absolute_arch_filepath, result_filename);
            }
            QMessageBox message(QMessageBox::Information, "Success", compress_ratio);
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

void ArchiveWindow::fileFormatExceptionHandler(QString _type, QString _message) {
    QMessageBox message(QMessageBox::Warning, _type, _message);
    message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    message.exec();
}
