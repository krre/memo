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
    TreeItem* childItem = item(parent)->child(row);

    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TreeModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeItem* parentItem = item(child)->parent();

    if (item(child)->parent() == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const {
    return item(parent)->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return 1;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    return item(index)->data();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return nullptr;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    item(index)->setData(value);

    emit dataChanged(index, index);

    return true;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex& parent) {
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = item(parent)->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex& parent) {
    TreeItem* parentItem = item(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

TreeItem* TreeModel::root() const {
    return rootItem;
}

TreeItem* TreeModel::item(const QModelIndex& index) const {
    if (index.isValid()) {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return rootItem;
}

QModelIndex TreeModel::index(TreeItem* item) const {
    if (item) {
        return createIndex(item->childNumber(), 0, item);
    } else {
        return QModelIndex();
    }
}

QVector<int> TreeModel::childIds(TreeItem* item) {
    QVector<int> ids;
    ids.append(item->id());

    for (int i = 0; i < item->childCount(); i++) {
        ids.append(childIds(item->child(i)));
    }

    return ids;
}
