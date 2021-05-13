#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(700, 500);
    w.setWindowTitle("PashukosZip");
    w.show();
    return a.exec();
}
