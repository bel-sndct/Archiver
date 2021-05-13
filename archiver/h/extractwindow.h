#ifndef EXTRACTWINDOW_H
#define EXTRACTWINDOW_H

#include <QDialog>

namespace Ui {
class ExtractWindow;
}

class ExtractWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractWindow(QWidget *parent = nullptr);
    void setFilePathToExtLineEdit_from(QString);
    ~ExtractWindow();

private slots:
    void on_pushButton_cancel_clicked();

private:
    Ui::ExtractWindow *ui;
};

#endif // EXTRACTWINDOW_H
