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
        QJsonObject obj;
        obj["version"] = update.version;
        obj["date"] = update.date;
        obj["size"] = update.size;
        obj["channel"] = update.channel;
        obj["description"] = update.description;

        QJsonArray os;
        for (const QString& value : update.os) {
            os.append(value);
        }

        obj["os"] = os;
        list.append(obj);
    }

    return list;
}

void ListModel::fromJson(const QJsonArray& json) {
    for (int i = json.count() - 1; i >= 0; i--) {
        QJsonObject obj = json.at(i).toObject();

        Update update;
        update.version = obj["version"].toString();
        update.date = obj["date"].toString();
        update.size = obj["size"].toInt();
        update.channel = obj["channel"].toString();
        update.description = obj["description"].toString();

        for (const auto osValue : obj["os"].toArray()) {
            update.os.append(osValue.toString());
        }

        addUpdate(update);
    }
}
