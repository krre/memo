#include "ListModel.h"
#include <QJsonArray>
#include <QJsonObject>
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

void ListModel::removeUpdate(int row) {
    if (row < 0 || row > updates.count() - 1) return;

    beginRemoveRows(QModelIndex(), row, row);
    updates.removeAt(row);
    endRemoveRows();
}

const ListModel::Update& ListModel::update(int row) const {
    return updates.at(row);
}

void ListModel::setUpdate(int row, const ListModel::Update& update) {
    if (row < 0 || row > updates.count() - 1) return;

    updates[row] = update;
    emit dataChanged(index(row), index(row));
}

void ListModel::clear() {
    int count = rowCount();

    for (int i = 0; i < count; i++) {
        removeUpdate(0);
    }
}

QJsonArray ListModel::toJson() {
    QJsonArray list;

    for (const Update& update : updates) {
        QJsonObject updateObj;
        updateObj["version"] = update.version;
        updateObj["baseVersion"] = update.baseVersion;
        updateObj["date"] = update.date;
        updateObj["channel"] = update.channel;
        updateObj["description"] = update.description;

        QJsonObject sizeObj;
        for (const auto& os : update.size.keys()) {
            sizeObj[os] = update.size[os];
        }

        updateObj["size"] = sizeObj;
        list.append(updateObj);
    }

    return list;
}

void ListModel::fromJson(const QJsonArray& json) {
    clear();

    for (int i = json.count() - 1; i >= 0; i--) {
        QJsonObject updateObj = json.at(i).toObject();

        Update update;
        update.version = updateObj["version"].toString();
        update.baseVersion = updateObj["baseVersion"].toString();
        update.date = updateObj["date"].toString();
        update.channel = updateObj["channel"].toString();
        update.description = updateObj["description"].toString();

        for (const auto& os : updateObj["size"].toObject().keys()) {
            update.size[os] = updateObj["size"].toObject()[os].toInt();
        }

        addUpdate(update);
    }
}
