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

void ArchiveWindow::setFilePathToArchLineEdit_from(QString _file_path) {
    this->ui->lineEdit_from->setText(_file_path);
}

void ArchiveWindow::on_pushButton_cancel_clicked() {
    close();
}
