#include "Outliner.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "../../database/Database.h"
#include <QtWidgets>

Outliner::Outliner(Database* database) : database(database) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &Outliner::onCustomContextMenu);
    createContextMenu();
    header()->setVisible(false);

    clear();

    connect(itemDelegate(), &QAbstractItemDelegate::closeEditor, [=] {
        TreeItem* item = model->item(selectionModel()->currentIndex());
        database->updateValue(item->id(), "title", item->data());
    });
}

void Outliner::updateActions() {
    bool hasCurrent = selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        closePersistentEditor(selectionModel()->currentIndex());
    }
}

void Outliner::build() {
    clear();
    QVector<Database::Title> titles = database->titles();
    int selectedId = database->metaValue("selected_id").toInt();

    TreeItem* rootItem = model->root();

    for (const Database::Title& title : titles) {
        TreeItem* parentItem = rootItem->find(title.parentId);
        QModelIndex parentIndex = model->index(parentItem);
        model->insertRow(title.pos, parentIndex);

        QModelIndex index = model->index(title.pos, 0, parentIndex);
        model->setData(index, QVariant(title.title), Qt::EditRole);
        model->item(index)->setId(title.id);
    }

    if (selectedId == 0) {
        setCurrentIndex(QModelIndex());
    } else {
        TreeItem* item = model->root()->find(selectedId);
        QModelIndex index = model->index(item);
        setCurrentIndex(index);

        TreeItem* parent = item->parent();

        while (parent != rootItem) {
            setExpanded(model->index(parent), true);
            parent = parent->parent();
        }
    }
}

void Outliner::clear() {
    isInited = false;

    if (model) {
        delete model;
    }

    model = new TreeModel;
    setModel(model);

    isInited = true;
}

void Outliner::onCustomContextMenu(const QPoint& point) {
    if (database->isOpen()) {
        updateContextMenu();
        contextMenu->exec(mapToGlobal(point));
    }
}

void Outliner::addNote() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Note"), tr("Name:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        insertChild(text);
    }
}

void Outliner::removeNotes() {
    QModelIndex index = selectionModel()->currentIndex();
    TreeItem* parentItem = model->item(index.parent());

    int result = QMessageBox::question(this, tr("Remove Notes"), tr("Remove %1?").arg(model->data(index).toString()));
    if (result == QMessageBox::Yes) {
        TreeItem* item = model->item(index);
        QVector<int> ids = model->childIds(item);
        model->removeRow(index.row(), index.parent());

        for (int id : ids) {
            database->removeRecord(id);
        }

        for (int i = 0; i < parentItem->childCount(); i++) {
            int id = parentItem->child(i)->id();
            database->updateValue(id, "pos", i);
        }
    }
}

void Outliner::renameNote() {
    edit(selectionModel()->currentIndex());
}

void Outliner::moveUp() {
    int row = currentIndex().row();
    model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row - 1);
}

void Outliner::moveDown() {
    int row = currentIndex().row();
    model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row + 2);
}

void Outliner::createContextMenu() {
    contextMenu = new QMenu(this);
    contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
    removeAction = contextMenu->addAction(tr("Remove..."), this, &Outliner::removeNotes);
    renameAction = contextMenu->addAction(tr("Rename"), this, &Outliner::renameNote);
    moveUpAction = contextMenu->addAction(tr("Move Up"), this, &Outliner::moveUp);
    moveDownAction = contextMenu->addAction(tr("Move Down"), this, &Outliner::moveDown);
}

void Outliner::updateContextMenu() {
    bool enabled = currentIndex().isValid();
    removeAction->setEnabled(enabled);
    renameAction->setEnabled(enabled);

    moveUpAction->setEnabled(enabled && currentIndex().row() > 0);
    moveDownAction->setEnabled(enabled && currentIndex().row() < model->rowCount(currentIndex().parent()) - 1);
}

void Outliner::insertChild(const QString& title) {
    QModelIndex currentIndex = selectionModel()->currentIndex();
    TreeItem* currentItem = model->item(currentIndex);

    int currentId = currentItem->id();
    int childRow = currentItem->childCount();
    int childDepth = currentItem->depth();
    int childId = database->insertRecord(currentId, childRow, childDepth, title);

    if (!model->insertRow(childRow, currentIndex)) {
        return;
    }

    QModelIndex child = model->index(childRow, 0, currentIndex);
    model->setData(child, QVariant(title), Qt::EditRole);
    model->item(child)->setId(childId);

    selectionModel()->setCurrentIndex(model->index(childRow, 0, currentIndex), QItemSelectionModel::ClearAndSelect);
    setExpanded(currentIndex, true);

    updateActions();
}


void Outliner::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !indexAt(event->localPos().toPoint()).isValid()) {
        selectionModel()->clearSelection();
        setCurrentIndex(QModelIndex());
    } else {
        QTreeView::mousePressEvent(event);
    }
}

void Outliner::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
    Q_UNUSED(previous)
    if (!isInited) return;

    int id = model->item(current)->id();
    database->updateMetaValue("selected_id", id);
    emit noteChanged(id);
}
