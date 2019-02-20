#pragma once
#include <QList>
#include <QVariant>
#include <QVector>

class TreeItem {

public:
    explicit TreeItem(TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem* child(int number);
    int childCount() const;
    QVariant data() const;
    bool insertChildren(int position, int count);
    TreeItem* parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    void setData(const QVariant& value);

    int id() const;
    void setId(int id);

    int depth();

private:
    QList<TreeItem*> childItems;
    QVariant itemData;
    TreeItem* parentItem = nullptr;
    int itemId = 0;
};
