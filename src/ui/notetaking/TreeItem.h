#pragma once
#include "core/Globals.h"
#include <QList>
#include <QVariant>

class TreeItem {

public:
    explicit TreeItem(TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem* parent();
    void setParent(TreeItem* parent);

    TreeItem* find(Id id);

    TreeItem* child(int number);
    int childCount() const;
    int childNumber() const;

    QVariant data() const;
    void setData(const QVariant& data);

    bool insertChild(int position, TreeItem* item = nullptr);
    bool removeChild(int position);

    Id id() const;
    void setId(Id id);

    int depth();

private:
    QList<TreeItem*> m_children;
    QVariant m_data;
    TreeItem* m_parent = nullptr;
    Id m_id = 0;
};
