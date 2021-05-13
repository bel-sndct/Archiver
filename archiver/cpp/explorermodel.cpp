#include "explorermodel.h"

ExplorerModel::ExplorerModel(QObject *parent)
    : QAbstractListModel(parent) {}

int ExplorerModel::rowCount(const QModelIndex&) const {
    return this->dir_list->count();
}

void ExplorerModel::getFolderList(QString _folder_path,
                                           QFileInfoList* _dir_list) {
    QDir dir = QDir(_folder_path);
    *_dir_list = dir.entryInfoList(QDir::NoDot | QDir::Files | QDir::Dirs, QDir::DirsFirst);

    this->beginResetModel();
    this->dir_list = _dir_list;
    this->endResetModel();
}

QVariant ExplorerModel::data(const QModelIndex &_index,
                                      int _role) const {
    QVariant value;

    switch (_role) {
        case Qt::DisplayRole: { //string
            value = this->dir_list->at(_index.row()).fileName();
            break;
        }
        case Qt::DecorationRole: {  //icon

            if (this->dir_list->at(_index.row()).isDir()) {
                QPixmap icon = QPixmap(":/res/img/folder.png");
                QPixmap tmp = icon.scaled(30, 30, Qt::KeepAspectRatio);
                value = tmp;
                break;
            }

            if (this->dir_list->at(_index.row()).isFile()) {
                QString file_ext = this->dir_list->at(_index.row()).completeSuffix();

                if (file_ext == "zip" || file_ext == "rar" || file_ext == "7z") {
                    QPixmap icon = QPixmap(":/res/img/arch.png");
                    QPixmap tmp = icon.scaled(30, 30, Qt::KeepAspectRatio);
                    value = tmp;
                    break;
                }

                QPixmap icon = QPixmap(":/res/img/fileicon.png");
                QPixmap tmp = icon.scaled(30, 30, Qt::KeepAspectRatio);
                value = tmp;
                break;
            }

            value = this->dir_list->at(_index.row()).fileName();
            break;
        }
        case Qt::UserRole: {    //data
            value = this->dir_list->at(_index.row()).fileName();
            break;
        }
        default:
            break;
    }

    return value;
}
