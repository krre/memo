#pragma once
#include <QAbstractListModel>

class ListModel : public QAbstractListModel {
    Q_OBJECT
public:
    struct Update {
        QString version;
        QString baseVersion;
        QString description;
        QString date;
        QMap<QString, qint64> size;
        QString channel;
    };

    ListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void addUpdate(const Update& update);
    void removeUpdate(int row);
    const Update& update(int row) const;
    void setUpdate(int row, const Update& update);
    void clear();

    QJsonArray toJson();
    void fromJson(const QJsonArray& json);

private:
    QList<Update> updates;
};
