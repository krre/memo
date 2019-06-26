#include "TreeItem.h"
#include <QStringList>
#include <QModelIndex>

TreeItem::TreeItem(TreeItem* parent) : m_parent(parent) {
}

TreeItem::~TreeItem() {
    qDeleteAll(m_childItems);
}

TreeItem* TreeItem::child(int number) {
    return m_childItems.value(number);
}

int TreeItem::childCount() const {
    return m_childItems.count();
}

int TreeItem::childNumber() const {
    if (m_parent) {
        return m_parent->m_childItems.indexOf(const_cast<TreeItem*>(this));
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

    for (TreeItem* child : m_childItems) {
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
    m_childItems.insert(position, childItem);

    return true;
}

bool TreeItem::removeChild(int position) {
    if (position < 0 || position > m_childItems.count() - 1) return false;

    m_childItems.removeAt(position);

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
