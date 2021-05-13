#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDesktopServices>
#include <QMainWindow>
#include <QListView>
#include <QUrl>
#include "explorermodel.h"
#include "archivewindow.h"
#include "extractwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAdd_to_archive_triggered();

    void on_actionExtract_data_from_archive_triggered();

    void on_actionRemove_file_triggered();

    void on_actionExit_triggered();

    void on_pushButton_undo_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QFileInfoList* dir_list;
    ExplorerModel* explorer_model;
    QDir current_folder;
    QString clicked_file;
};
#endif // MAINWINDOW_H
