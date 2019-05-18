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

const ListModel::Update& ListModel::getUpdate(int row) const {
    return updates.at(row);
}

void ListModel::setUpdate(int row, const ListModel::Update& update) {
    if (row < 0 || row > updates.count() - 1) return;

    updates[row] = update;
    emit dataChanged(index(row), index(row));
}

QJsonArray ListModel::toJson() {
    QJsonArray list;

    for (const Update& update : updates) {
        QJsonObject updateObj;
        updateObj["version"] = update.version;
        updateObj["date"] = update.date;
        updateObj["channel"] = update.channel;
        updateObj["description"] = update.description;

        QJsonObject sizeObj;
        QMap<QString, qint64>::const_iterator i = update.size.constBegin();

        while (i != update.size.constEnd()) {
            sizeObj[i.key()] = i.value();
            ++i;
        }

        updateObj["size"] = sizeObj;
        list.append(updateObj);
    }

    return list;
}

void ListModel::fromJson(const QJsonArray& json) {
    for (int i = json.count() - 1; i >= 0; i--) {
        QJsonObject updateObj = json.at(i).toObject();

        Update update;
        update.version = updateObj["version"].toString();
        update.date = updateObj["date"].toString();
        update.channel = updateObj["channel"].toString();
        update.description = updateObj["description"].toString();

        for (const auto& os : updateObj["size"].toObject().keys()) {
            update.size[os] = updateObj["size"].toObject()[os].toInt();
        }

        addUpdate(update);
    }
}
