#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {

    ui->setupUi(this);

    setWindowIcon(QIcon(":/res/img/main_icon.png"));

    this->dir_list = new QFileInfoList();
    this->explorer_model = new ExplorerModel();

    QString cur_fol = "/";
    this->current_folder = cur_fol;
    this->explorer_model->getFolderList("/", this->dir_list);           //this->current_folder.absolutePath()

    this->ui->listView->setModel(explorer_model);
    this->ui->lineEdit->setText("/");
    this->ui->lineEdit->setReadOnly(true);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionAdd_to_archive_triggered() {
    //QString path = this->ui->lineEdit->text();
    QStringList path_list;
    if (this->clicked_file.size()) {
        path_list = this->clicked_file.split(".");
    }

    ArchiveWindow* arch_window = new ArchiveWindow;

    if (this->clicked_file.size() && (path_list.back() == "txt"
                                   || path_list.back() == "cpp"
                                   || path_list.back() == "jpg"
                                   || path_list.back() == "xml"
                                   || path_list.back() == "file")) {

        ui->statusbar->showMessage("Adding a new archive");

        arch_window->setFixedSize(520, 300);
        arch_window->setModal(true);
        arch_window->setWindowTitle("New archive");
        arch_window->setFilePathToArchLineEdit_from(clicked_file);
        arch_window->setCurrentDirectory(this->current_folder);
        arch_window->setHuffRoot(this->huffman_processor.getRoot());
        arch_window->exec();
    }
    else {
        QMessageBox message(QMessageBox::Warning, "ERROR", "Choose .txt/.cpp/.java/.docx/.xml/.html file");
        message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        message.exec();
    }
    this->explorer_model->getFolderList(current_folder.absolutePath(), this->dir_list);
    this->huffman_processor.setRoot(arch_window->getHuffmanProcessor());
    this->deflate_processor.setDefRoot(arch_window->getDeflateProcessor().getDefRoot());
    if (this->clicked_file.size() != 0) this->clicked_file.clear();
    arch_window->~ArchiveWindow();
}

void MainWindow::on_actionExtract_data_from_archive_triggered() {
    //QString path = this->ui->lineEdit->text();
    QStringList path_list;
    if (this->clicked_file.size()) {
        path_list = this->clicked_file.split(".");
    }

    ExtractWindow* ext_window = new ExtractWindow;

    if (this->clicked_file.size() && (path_list.back() == "huff"
                                   || path_list.back() == "lz")) {

        ui->statusbar->showMessage("Extracting data from archive");

        ext_window->setFixedSize(520, 300);
        ext_window->setModal(true);
        ext_window->setWindowTitle("Extracting data");
        ext_window->setFilePathToExtLineEdit_from(clicked_file);
        ext_window->setCurrentDirectory(this->current_folder);
        ext_window->setHuffRoot(this->huffman_processor.getRoot());
        ext_window->setDeflateProcessor(this->deflate_processor.getDefRoot());
        ext_window->exec();
    }
    else {
        QMessageBox message(QMessageBox::Warning, "ERROR", "Choose .huff/.lz file");
        message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        message.exec();
    }
    this->explorer_model->getFolderList(current_folder.absolutePath(), this->dir_list);
    if (this->clicked_file.size() != 0) this->clicked_file.clear();
    //this->huffman_processor.setRoot(ext_window->getHuffmanProcessor());
    ext_window->~ExtractWindow();
}

void MainWindow::on_actionRemove_file_triggered() {

    ui->statusbar->showMessage("Removing file");
    if (this->clicked_file.size() != 0) {
        QFile(this->clicked_file).remove();
        this->explorer_model->getFolderList(current_folder.absolutePath(), this->dir_list);
    }
    else {
        QMessageBox message(QMessageBox::Warning, "Warning", "Choose file for removing");
        message.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        message.exec();
    }
}

void MainWindow::on_actionExit_triggered() {
    exit(0);
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
