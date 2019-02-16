#pragma once
#include <QAbstractItemModel>

class TreeItem;

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    TreeModel(QObject* parent = nullptr);
    ~TreeModel() override;

    QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    bool insertRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex& parent = QModelIndex()) override;

private:
    TreeItem* getItem(const QModelIndex& index) const;

    TreeItem* rootItem;
};
