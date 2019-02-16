#pragma once
#include <QAbstractItemModel>

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    TreeModel(QObject* parent = nullptr);
};
