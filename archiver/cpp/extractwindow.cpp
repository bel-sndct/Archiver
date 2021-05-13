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
    this->ui->lineEdit_from->setText(_file_path);
}

void ExtractWindow::on_pushButton_cancel_clicked() {
    close();
}
