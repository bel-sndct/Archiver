#ifndef ARCHIVEWINDOW_H
#define ARCHIVEWINDOW_H

#include <QDialog>

namespace Ui {
class ArchiveWindow;
}

class ArchiveWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ArchiveWindow(QWidget *parent = nullptr);
    void setFilePathToArchLineEdit_from(QString);
    ~ArchiveWindow();

private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::ArchiveWindow *ui;
};

#endif // ARCHIVEWINDOW_H
