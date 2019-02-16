#include "TreeModel.h"
#include "TreeItem.h"
#include <QtCore>

TreeModel::TreeModel(QObject* parent) : QAbstractItemModel (parent) {
    rootItem = new TreeItem;
}

TreeModel::~TreeModel() {
    delete rootItem;
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const {
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& child) const {
    return QModelIndex();
}

int TreeModel::rowCount(const QModelIndex& parent) const {
    return 0;
}

int TreeModel::columnCount(const QModelIndex& parent) const {
    return 0;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
    return QVariant();
}
