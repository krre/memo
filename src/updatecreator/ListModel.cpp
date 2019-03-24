#include "ListModel.h"
#include <QDebug>

ListModel::ListModel(QObject* parent) : QAbstractListModel(parent) {

}

int ListModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return updates.count();
}

QVariant ListModel::data(const QModelIndex& index, int role) const {
    Q_UNUSED(role)

    if (!index.isValid()) return QVariant();
    if (index.row() >= updates.size() || index.row() < 0) return QVariant();

    if (role == Qt::DisplayRole) {
        const auto &update = updates.at(index.row());
        return QVariant(QString("v%1 - %2").arg(update.version, update.date));
    }

    return QVariant();
}

void ListModel::addUpdate(const ListModel::Update& update) {
    beginInsertRows(QModelIndex(), 0, 0);
    updates.insert(0, update);
    endInsertRows();
}
