#include "TreeItem.h"
#include <QStringList>
#include <QDebug>

TreeItem::TreeItem(const QVariant& data, TreeItem* parent) {
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem() {
    qDeleteAll(childItems);
}

TreeItem* TreeItem::child(int number) {
    return childItems.value(number);
}

int TreeItem::childCount() const {
    return childItems.count();
}

int TreeItem::childNumber() const {
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
    }

    return 0;
}

QVariant TreeItem::data() const {
    return itemData;
}

bool TreeItem::insertChildren(int position, int count) {
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        QVariant data;
        TreeItem* item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

TreeItem* TreeItem::parent() {
    return parentItem;
}

bool TreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        delete childItems.takeAt(position);
    }

    return true;
}

void TreeItem::setData(const QVariant& value) {
    itemData = value;
}
