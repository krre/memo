#include "TreeModel.h"
#include "TreeItem.h"
#include <QMimeData>
#include <QIODevice>

constexpr auto TreeItemMimeType = "application/x-treeitem";

TreeModel::TreeModel(QObject* parent) : QAbstractItemModel (parent) {
    m_rootItem.reset(new TreeItem);
}

TreeModel::~TreeModel() {
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const {
    auto childItem = item(parent)->child(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& child) const {
    if (!child.isValid()) {
        return QModelIndex();
    }

    auto parentItem = item(child)->parent();

    if (!parentItem || parentItem == m_rootItem.data()) {
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
    auto result = new QMimeData;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << item(indexes.first())->id().value();
    result->setData(TreeItemMimeType, data);
    return result;
}

bool TreeModel::canDropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row [[maybe_unused]], int column [[maybe_unused]], const QModelIndex& parent [[maybe_unused]]) const {
    if (action != Qt::MoveAction) return false;
    if (!mimeData->hasFormat(TreeItemMimeType)) return false;

    return true;
}

bool TreeModel::dropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row, int column, const QModelIndex& parent) {
    if (!canDropMimeData(mimeData, action, row, column, parent)) return false;

    QByteArray data = mimeData->data(TreeItemMimeType);
    QDataStream stream(&data, QIODevice::ReadOnly);

    quint64 rawId;
    stream >> rawId;

    Id id(rawId);

    auto sourceItem = m_rootItem->find(id);
    QModelIndex sourceParent = index(sourceItem->parent());

    bool result = moveRow(sourceParent, sourceItem->childNumber(), parent, row);

    auto validParent = parent.isValid() ? parent : index(m_rootItem.data());
    int parentChildCount = item(validParent)->childCount();
    int validRow = row >= 0 && row < parentChildCount ? row : parentChildCount - 1;

    emit itemDropped(index(validRow, 0, validParent));

    return result;
}

bool TreeModel::insertRows(int position, int rows [[maybe_unused]], const QModelIndex& parent) {
    beginInsertRows(parent, position, position);
    bool success = item(parent)->insertChild(position);
    endInsertRows();

    return success;
}

bool TreeModel::removeRows(int position, int rows [[maybe_unused]], const QModelIndex& parent) {
    beginRemoveRows(parent, position, position);
    auto removedItem = item(parent)->removeChild(position);

    if (removedItem) {
        delete removedItem;
    }

    endRemoveRows();

    return removedItem;
}

bool TreeModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count [[maybe_unused]], const QModelIndex& destinationParent, int destinationChild) {
    auto destinationParentItem = destinationParent.isValid() ? item(destinationParent) : m_rootItem.data();
    int destinationRow = destinationChild >= 0 ? destinationChild : destinationParentItem->childCount();

    beginMoveRows(sourceParent, sourceRow, sourceRow, destinationParent, destinationRow);

    auto sourceItem = item(sourceParent)->removeChild(sourceRow);
    destinationParentItem->insertChild(destinationRow, sourceItem);

    endMoveRows();

    return false; // Need false to disable removing row by Qt.
}

TreeItem* TreeModel::root() const {
    return m_rootItem.data();
}

TreeItem* TreeModel::item(const QModelIndex& index) const {
    return index.isValid() && index.internalPointer() ? static_cast<TreeItem*>(index.internalPointer()) : m_rootItem.data();
}

QModelIndex TreeModel::index(TreeItem* item) const {
    return item ? createIndex(item->childNumber(), 0, item) : QModelIndex();
}

Ids TreeModel::childIds(TreeItem* item) const {
    Ids result;
    result.append(item->id());

    for (int i = 0; i < item->childCount(); ++i) {
        result.append(childIds(item->child(i)));
    }

    return result;
}
