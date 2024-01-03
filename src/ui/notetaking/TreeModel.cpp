#include "TreeModel.h"
#include "TreeItem.h"
#include <QtCore>

constexpr auto TreeItemMimeType = "application/x-treeitem";

TreeModel::TreeModel(QObject* parent) : QAbstractItemModel (parent) {
    m_rootItem.reset(new TreeItem);
}

TreeModel::~TreeModel() {
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const {
    TreeItem* childItem = item(parent)->child(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    TreeItem* parentItem = item(child)->parent();

    if (item(child)->parent() == m_rootItem.data()) {
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const {
    return item(parent)->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent [[maybe_unused]]) const {
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
        return Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
    }

    return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    item(index)->setData(value);

    emit dataChanged(index, index);

    return true;
}

Qt::DropActions TreeModel::supportedDropActions() const {
    return Qt::MoveAction;
}

Qt::DropActions TreeModel::supportedDragActions() const {
    return Qt::MoveAction;
}

QStringList TreeModel::mimeTypes() const {
    return { TreeItemMimeType };
}

QMimeData* TreeModel::mimeData(const QModelIndexList& indexes) const {
    auto mimeData = new QMimeData;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << item(indexes.first())->id();
    mimeData->setData(TreeItemMimeType, data);
    return mimeData;
}

bool TreeModel::canDropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row [[maybe_unused]], int column [[maybe_unused]], const QModelIndex& parent [[maybe_unused]]) const {
    if (action != Qt::MoveAction) return false;
    if (!mimeData->hasFormat(TreeItemMimeType)) return false;

    return true;
}

bool TreeModel::dropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row, int column [[maybe_unused]], const QModelIndex& parent) {
    if (!canDropMimeData(mimeData, action, row, column, parent)) return false;

    QByteArray data = mimeData->data(TreeItemMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);
    Id id;
    stream >> id;

    TreeItem* sourceItem = m_rootItem->find(id);
    QModelIndex sourceParent = index(sourceItem->parent());

    if (row < 0) {
        if (parent.isValid()) {
            row = 0;
        } else if (sourceItem->parent() == m_rootItem.data()) {
            row = rowCount(parent) - 1;
        } else {
            row = rowCount(parent);
        }
    } else if (sourceItem->parent() == item(parent) && sourceItem->childNumber() < row) {
        row--;
    }

    removeRow(sourceItem->childNumber(), sourceParent);

    beginInsertRows(parent, row, row);
    item(parent)->insertChild(row, sourceItem);
    endInsertRows();

    emit itemDropped(index(row, 0, parent));

    return false; // Need false to disable removing row by Qt.
}

bool TreeModel::insertRows(int position, int rows [[maybe_unused]], const QModelIndex& parent) {
    beginInsertRows(parent, position, position);
    bool success = item(parent)->insertChild(position);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows [[maybe_unused]], const QModelIndex& parent) {
    beginRemoveRows(parent, position, position);
    bool success = item(parent)->removeChild(position);
    endRemoveRows();

    return success;
}

bool TreeModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count [[maybe_unused]], const QModelIndex& destinationParent, int destinationChild) {
    bool success = false;

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationChild);
    TreeItem* sourceParentItem = item(sourceParent);
    TreeItem* targetItem = sourceParentItem->child(sourceRow);

    success = item(destinationParent)->insertChild(destinationChild, targetItem);

    if (success) {
        if (sourceParent == destinationParent) {
            if (sourceRow > destinationChild) {
                success = sourceParentItem->removeChild(sourceRow + 1);
            } else {
                success = sourceParentItem->removeChild(sourceRow);
            }
        }
    }

    endMoveRows();

    return success;
}

TreeItem* TreeModel::root() const {
    return m_rootItem.data();
}

TreeItem* TreeModel::item(const QModelIndex& index) const {
    if (index.isValid()) {
        auto item = static_cast<TreeItem*>(index.internalPointer());

        if (item) {
            return item;
        }
    }

    return m_rootItem.data();
}

QModelIndex TreeModel::index(TreeItem* item) const {
    return item ? createIndex(item->childNumber(), 0, item) : QModelIndex();
}

QVector<Id> TreeModel::childIds(TreeItem* item) {
    QVector<Id> result;
    result.append(item->id());

    for (int i = 0; i < item->childCount(); i++) {
        result.append(childIds(item->child(i)));
    }

    return result;
}
