#pragma once
#include "core/Id.h"
#include <QList>
#include <QVariant>

class TreeItem {
public:
    TreeItem(TreeItem* parent = nullptr);
    ~TreeItem();

    TreeItem* parent();
    void setParent(TreeItem* parent);

    TreeItem* find(Id id);

    TreeItem* child(int number) const;
    int childCount() const;
    int childNumber() const;

    QVariant data() const;
    void setData(const QVariant& data);

    bool insertChild(int position, TreeItem* item = nullptr);
    TreeItem* removeChild(int position);

    Id id() const;
    void setId(Id id);

    int depth();

private:
    QList<TreeItem*> m_children;
    QVariant m_data;
    TreeItem* m_parent = nullptr;
    Id m_id = 0;
};
