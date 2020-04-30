#include "TreeItem.h"
#include <QStringList>
#include <QModelIndex>

TreeItem::TreeItem(TreeItem* parent) : m_parent(parent) {
}

TreeItem::~TreeItem() {
    qDeleteAll(m_children);
}

TreeItem* TreeItem::child(int number) {
    return m_children.value(number);
}

int TreeItem::childCount() const {
    return m_children.count();
}

int TreeItem::childNumber() const {
    if (m_parent) {
        return m_parent->m_children.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}

QVariant TreeItem::data() const {
    return m_data;
}

TreeItem* TreeItem::parent() {
    return m_parent;
}

void TreeItem::setParent(TreeItem* parent) {
    m_parent = parent;
}

TreeItem* TreeItem::find(Id id) {
    if (m_id == id) return this;

    for (TreeItem* child : m_children) {
        TreeItem* item = child->find(id);

        if (item) {
            return item;
        }
    }

    return nullptr;
}

bool TreeItem::insertChild(int position, TreeItem* item) {
    TreeItem* childItem = item ? item : new TreeItem;
    childItem->setParent(this);
    m_children.insert(position, childItem);

    return true;
}

bool TreeItem::removeChild(int position) {
    if (position < 0 || position > m_children.count() - 1) return false;

    m_children.removeAt(position);

    return true;
}

void TreeItem::setData(const QVariant& data) {
    m_data = data;
}

Id TreeItem::id() const {
    return m_id;
}

void TreeItem::setId(Id id) {
    m_id = id;
}

int TreeItem::depth() {
    int counter = 0;
    TreeItem* item = this;

    while (item->id() != 0) {
        counter++;
        item = item->parent();
    }

    return counter;
}
