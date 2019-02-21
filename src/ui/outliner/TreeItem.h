#pragma once
#include <QList>
#include <QVariant>
#include <QVector>

class TreeItem {

public:
    explicit TreeItem(TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem* parent();
    TreeItem* find(int id);

    TreeItem* child(int number);
    int childCount() const;
    int childNumber() const;

    QVariant data() const;
    void setData(const QVariant& value);

    bool insertChildren(int position, int count);
    bool removeChildren(int position, int count);

    int id() const;
    void setId(int id);

    int depth();

private:
    QList<TreeItem*> childItems;
    QVariant itemData;
    TreeItem* parentItem = nullptr;
    int itemId = 0;
};
