#include "TreeItem.h"
#include <QStringList>
#include <QModelIndex>

TreeItem::TreeItem(TreeItem* parent) : m_parent(parent) {
}

TreeItem::~TreeItem() {
    qDeleteAll(m_children);
}

TreeItem* TreeItem::child(int number) const {
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

    for (auto child : std::as_const(m_children)) {
        auto item = child->find(id);

        if (item) {
            return item;
        }
    }

    return nullptr;
}

bool TreeItem::insertChild(int position, TreeItem* item) {
    auto childItem = item ? item : new TreeItem;
    childItem->setParent(this);

    if (position >= m_children.count()) {
        m_children.append(childItem);
    } else {
        m_children.insert(position, childItem);
    }

    return true;
}

TreeItem* TreeItem::removeChild(int position) {
    if (position < 0 || position > m_children.count() - 1) return nullptr;

    auto item = m_children.at(position);
    m_children.removeAt(position);

    return item;
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
    auto item = this;

    while (item->id() != 0) {
        counter++;
        item = item->parent();
    }

    return counter;
}
