#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    this->dir_list = new QFileInfoList();
    this->explorer_model = new ExplorerModel();

    this->current_folder = QDir::currentPath();
    this->explorer_model->getFolderList(this->current_folder.absolutePath(), this->dir_list);

    this->ui->listView->setModel(explorer_model);
    this->ui->lineEdit->setText(QDir::currentPath());
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_actionAdd_to_archive_triggered() {

    ui->statusbar->showMessage("Adding a new archive");
    ArchiveWindow* arch_window = new ArchiveWindow;

    arch_window->setFixedSize(520, 300);
    arch_window->setModal(true);
    arch_window->setWindowTitle("New archive");
    arch_window->setFilePathToArchLineEdit_from(clicked_file);
    arch_window->exec();
}

void MainWindow::on_actionExtract_data_from_archive_triggered() {

    ui->statusbar->showMessage("Extracting data from archive");
    ExtractWindow* ext_window = new ExtractWindow;

    ext_window->setFixedSize(520, 300);
    ext_window->setModal(true);
    ext_window->setWindowTitle("Extracting data");
    ext_window->setFilePathToExtLineEdit_from(clicked_file);
    ext_window->exec();
}

void MainWindow::on_actionRemove_file_triggered() {
    ui->statusbar->showMessage("Removing file");
}

void MainWindow::on_actionExit_triggered() {
    close();
}

void MainWindow::on_pushButton_undo_clicked() {
    this->current_folder.cdUp();
    this->ui->lineEdit->setText(current_folder.absolutePath());
    this->explorer_model->getFolderList(current_folder.absolutePath(), this->dir_list);
}

void MainWindow::on_listView_clicked(const QModelIndex &index) {
    if (!this->dir_list->at(index.row()).isDir()) {
        this->clicked_file = dir_list->at(index.row()).absoluteFilePath();
    }
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index) {
    if (this->dir_list->at(index.row()).isDir()) {
        QString tmp = this->dir_list->at(index.row()).absoluteFilePath();
        this->explorer_model->getFolderList(this->dir_list->at(index.row()).absoluteFilePath(), this->dir_list);
        this->current_folder = tmp;
        this->ui->lineEdit->setText(tmp);
    }
    else {
        QDesktopServices::openUrl(QUrl(this->dir_list->at(index.row()).absoluteFilePath()));
    }
}
