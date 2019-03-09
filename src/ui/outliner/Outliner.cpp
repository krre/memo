#include "Outliner.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "NoteProperties.h"
#include "database/Database.h"
#include "core/Constants.h"
#include <QtWidgets>
#include <QtSql>

Outliner::Outliner(Database* database) : database(database) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &Outliner::onCustomContextMenu);
    createContextMenu();
    header()->setVisible(false);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);

    clear();

    connect(itemDelegate(), &QAbstractItemDelegate::closeEditor, [=] {
        TreeItem* item = model->item(selectionModel()->currentIndex());
        database->updateValue(item->id(), "title", item->data());
    });
}

void Outliner::exportAllNotes(const QString& directory) {
    QDateTime dateTime = QDateTime::currentDateTime();
    QString path = directory + "/Memo-" + dateTime.date().toString("yyyy-MM-dd") + "_" + dateTime.time().toString("HH-mm-ss");
    QDir dir;
    dir.mkdir(path);

    int count = exportNote(0, path);

    QMessageBox::information(this, tr("Export Finished"), tr("Count of notes: %1").arg(count));
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

    delete model;

    model = new TreeModel;
    setModel(model);
    connect(model, &TreeModel::itemDropped, this, &Outliner::moveTree);

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

    int id1 = model->item(currentIndex())->id();
    int id2 = model->item(currentIndex().sibling(row - 1, 0))->id();

    model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row - 1);

    database->updateValue(id1, "pos", row - 1);
    database->updateValue(id2, "pos", row);
}

void Outliner::moveDown() {
    int row = currentIndex().row();

    int id1 = model->item(currentIndex())->id();
    int id2 = model->item(currentIndex().sibling(row + 1, 0))->id();

    model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row + 2);

    database->updateValue(id1, "pos", row + 1);
    database->updateValue(id2, "pos", row);
}

void Outliner::moveTree(const QModelIndex& index) {
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    expand(index.parent());

    // Move tree in database.
    TreeItem* targetItem = model->item(index);
    int sourceId = targetItem->id();
    int sourceParentId = database->value(sourceId, "parent_id").toInt();
    int destinationParentId = targetItem->parent()->id();

    TreeItem* sourceParentItem = model->root()->find(sourceParentId);

    // Rewrite note positions on source parent.
    for (int i = 0; i < sourceParentItem->childCount(); i++) {
         database->updateValue(sourceParentItem->child(i)->id(), "pos", i);
    }

    if (sourceParentId != destinationParentId) {
         database->updateValue(sourceId, "parent_id", destinationParentId);

         TreeItem* destinationParentItem = model->root()->find(destinationParentId);

         // Rewrite note positions on destination parent.
         for (int i = 0; i < destinationParentItem->childCount(); i++) {
             database->updateValue(destinationParentItem->child(i)->id(), "pos", i);
         }

         // Rewrite depth in all children of target note.
         QVector<int> childIds = model->childIds(targetItem);
         for (int id : childIds) {
             int depth = targetItem->find(id)->depth() - 1;
             database->updateValue(id, "depth", depth);
         }
    }
}

void Outliner::showProperties() {
    int id = model->item(currentIndex())->id();
    QSqlQuery query = database->record(id);

    NoteProperties::Data data;
    data.title = query.value("title").toString();
    data.createdAt = query.value("created_at").toString();
    data.updatedAt = query.value("updated_at").toString();

    NoteProperties props(data);
    props.exec();
}

void Outliner::createContextMenu() {
    contextMenu = new QMenu(this);
    contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
    removeAction = contextMenu->addAction(tr("Remove..."), this, &Outliner::removeNotes);
    renameAction = contextMenu->addAction(tr("Rename"), this, &Outliner::renameNote);
    contextMenu->addSeparator();
    moveUpAction = contextMenu->addAction(tr("Move Up"), this, &Outliner::moveUp);
    moveDownAction = contextMenu->addAction(tr("Move Down"), this, &Outliner::moveDown);
    contextMenu->addSeparator();
    propertiesAction = contextMenu->addAction(tr("Properties..."), this, &Outliner::showProperties);
}

void Outliner::updateContextMenu() {
    bool enabled = currentIndex().isValid();
    removeAction->setEnabled(enabled);
    renameAction->setEnabled(enabled);

    moveUpAction->setEnabled(enabled && currentIndex().row() > 0);
    moveDownAction->setEnabled(enabled && currentIndex().row() < model->rowCount(currentIndex().parent()) - 1);

    propertiesAction->setEnabled(enabled);
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

int Outliner::exportNote(int parentId, const QString& path) {
    QDir dir;
    dir.mkdir(path);

    int count = 0;
    TreeItem* parentItem = model->root()->find(parentId);

    for (int i = 0; i < parentItem->childCount(); i++) {
        TreeItem* childItem = parentItem->child(i);
        QString title = childItem->data().toString();
        QString note = database->value(childItem->id(), "note").toString();
        QString notePath = path + "/" + title;

        QString filename = notePath + ".txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream.setCodec("UTF8");
            stream << note;
            file.close();
        }

        count++;

        if (childItem->childCount()) {
            count += exportNote(childItem->id(), notePath);
        }
    }

    return count;
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
