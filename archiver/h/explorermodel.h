#ifndef EXPLORERMODEL_H
#define EXPLORERMODEL_H

#include <QDir>
#include <QModelIndex>
#include <QPixmap>
#include <QString>

class ExplorerModel : public QAbstractListModel {
public:
    ExplorerModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex&) const;
    QVariant data(const QModelIndex& _index, int _role) const;

    void getFolderList(QString _folder_path, QFileInfoList* _dir_list);

private:
    QFileInfoList* dir_list;
};

#endif // EXPLORERMODEL_H
