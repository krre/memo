#pragma once
#include <QAbstractListModel>

class ListModel : public QAbstractListModel {
    Q_OBJECT
public:

    struct Update {
        QString version;
        QString description;
        QString date;
        QVector<QString> os;
        int size;
        QString channel;
    };

    ListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void addUpdate(const Update& update);
    void removeUpdate(int row);
    const Update& getUpdate(int row) const;
    void setUpdate(int row, const Update& update);

private:
    QList<Update> updates;
};
