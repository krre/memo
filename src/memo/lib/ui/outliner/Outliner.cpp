#include "Outliner.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "NoteProperties.h"
#include "database/Database.h"
#include "core/Constants.h"
#include "core/DatabaseException.h"
#include <QtWidgets>
#include <QtSql>

Outliner::Outliner(Database* database) : m_database(database) {
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
        TreeItem* item = m_model->item(selectionModel()->currentIndex());
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
    QVector<Database::Title> titles = m_database->titles();
    int selectedId = m_database->metaValue("selected_id").toInt();

    TreeItem* rootItem = m_model->root();

    for (const Database::Title& title : titles) {
        TreeItem* parentItem = rootItem->find(title.parentId);
        QModelIndex parentIndex = m_model->index(parentItem);
        m_model->insertRow(title.pos, parentIndex);

        QModelIndex index = m_model->index(title.pos, 0, parentIndex);
        m_model->setData(index, QVariant(title.title), Qt::EditRole);
        m_model->item(index)->setId(title.id);
    }

    if (selectedId == 0) {
        setCurrentIndex(QModelIndex());
    } else {
        TreeItem* item = m_model->root()->find(selectedId);
        QModelIndex index = m_model->index(item);
        setCurrentIndex(index);

        TreeItem* parent = item->parent();

        while (parent != rootItem) {
            setExpanded(m_model->index(parent), true);
            parent = parent->parent();
        }
    }
}

void Outliner::clear() {
    m_isInited = false;

    delete m_model;

    m_model = new TreeModel;
    setModel(m_model);
    connect(m_model, &TreeModel::itemDropped, this, &Outliner::moveTree);

    m_isInited = true;
}

void Outliner::onCustomContextMenu(const QPoint& point) {
    if (m_database->isOpen()) {
        updateContextMenu();
        m_contextMenu->exec(mapToGlobal(point));
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
    TreeItem* parentItem = m_model->item(index.parent());

    int result = QMessageBox::question(this, tr("Remove Notes"), tr("Remove %1?").arg(m_model->data(index).toString()));
    if (result == QMessageBox::Yes) {
        TreeItem* item = m_model->item(index);
        QVector<int> ids = m_model->childIds(item);
        m_model->removeRow(index.row(), index.parent());

        for (Id id : ids) {
            m_database->removeRecord(id);
        }

        for (int i = 0; i < parentItem->childCount(); i++) {
            Id id = parentItem->child(i)->id();
            m_database->updateValue(id, "pos", i);
        }
    }
}

void Outliner::renameNote() {
    edit(selectionModel()->currentIndex());
}

void Outliner::moveUp() {
    int row = currentIndex().row();

    Id id1 = m_model->item(currentIndex())->id();
    Id id2 = m_model->item(currentIndex().sibling(row - 1, 0))->id();

    m_model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row - 1);

    m_database->updateValue(id1, "pos", row - 1);
    m_database->updateValue(id2, "pos", row);
}

void Outliner::moveDown() {
    int row = currentIndex().row();

    Id id1 = m_model->item(currentIndex())->id();
    Id id2 = m_model->item(currentIndex().sibling(row + 1, 0))->id();

    m_model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row + 2);

    m_database->updateValue(id1, "pos", row + 1);
    m_database->updateValue(id2, "pos", row);
}

void Outliner::moveTree(const QModelIndex& index) {
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    expand(index.parent());

    // Move tree in database.
    TreeItem* targetItem = m_model->item(index);
    Id sourceId = targetItem->id();
    Id sourceParentId = m_database->value(sourceId, "parent_id").toInt();
    Id destinationParentId = targetItem->parent()->id();

    TreeItem* sourceParentItem = m_model->root()->find(sourceParentId);

    // Rewrite note positions on source parent.
    for (int i = 0; i < sourceParentItem->childCount(); i++) {
         m_database->updateValue(sourceParentItem->child(i)->id(), "pos", i);
    }

    if (sourceParentId != destinationParentId) {
         m_database->updateValue(sourceId, "parent_id", destinationParentId);

         TreeItem* destinationParentItem = m_model->root()->find(destinationParentId);

         // Rewrite note positions on destination parent.
         for (int i = 0; i < destinationParentItem->childCount(); i++) {
             m_database->updateValue(destinationParentItem->child(i)->id(), "pos", i);
         }

         // Rewrite depth in all children of target note.
         QVector<int> childIds = m_model->childIds(targetItem);
         for (Id id : childIds) {
             int depth = targetItem->find(id)->depth() - 1;
             m_database->updateValue(id, "depth", depth);
         }
    }
}

void Outliner::showProperties() {
    Id id = m_model->item(currentIndex())->id();
    QSqlQuery query = m_database->record(id);

    NoteProperties::Data data;
    data.title = query.value("title").toString();
    data.createdAt = query.value("created_at").toString();
    data.updatedAt = query.value("updated_at").toString();

    NoteProperties props(data);
    props.exec();
}

void Outliner::createContextMenu() {
    m_contextMenu = new QMenu(this);
    m_contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
    m_removeAction = m_contextMenu->addAction(tr("Remove..."), this, &Outliner::removeNotes);
    m_renameAction = m_contextMenu->addAction(tr("Rename"), this, &Outliner::renameNote);
    m_contextMenu->addSeparator();
    m_moveUpAction = m_contextMenu->addAction(tr("Move Up"), this, &Outliner::moveUp);
    m_moveDownAction = m_contextMenu->addAction(tr("Move Down"), this, &Outliner::moveDown);
    m_contextMenu->addSeparator();
    m_propertiesAction = m_contextMenu->addAction(tr("Properties..."), this, &Outliner::showProperties);
}

void Outliner::updateContextMenu() {
    bool enabled = currentIndex().isValid();
    m_removeAction->setEnabled(enabled);
    m_renameAction->setEnabled(enabled);

    m_moveUpAction->setEnabled(enabled && currentIndex().row() > 0);
    m_moveDownAction->setEnabled(enabled && currentIndex().row() < m_model->rowCount(currentIndex().parent()) - 1);

    m_propertiesAction->setEnabled(enabled);
}

void Outliner::insertChild(const QString& title) {
    QModelIndex currentIndex = selectionModel()->currentIndex();
    TreeItem* currentItem = m_model->item(currentIndex);

    Id currentId = currentItem->id();
    int childRow = currentItem->childCount();
    int childDepth = currentItem->depth();
    int childId = m_database->insertRecord(currentId, childRow, childDepth, title);

    if (!m_model->insertRow(childRow, currentIndex)) {
        return;
    }

    QModelIndex child = m_model->index(childRow, 0, currentIndex);
    m_model->setData(child, QVariant(title), Qt::EditRole);
    m_model->item(child)->setId(childId);

    selectionModel()->setCurrentIndex(m_model->index(childRow, 0, currentIndex), QItemSelectionModel::ClearAndSelect);
    setExpanded(currentIndex, true);

    updateActions();
}

int Outliner::exportNote(Id parentId, const QString& path) {
    QDir dir;
    dir.mkdir(path);

    int count = 0;
    TreeItem* parentItem = m_model->root()->find(parentId);

    for (int i = 0; i < parentItem->childCount(); i++) {
        TreeItem* childItem = parentItem->child(i);
        QString title = childItem->data().toString();
        QString note = m_database->value(childItem->id(), "note").toString();
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
    if (!m_isInited) return;

    try {
        Id id = m_model->item(current)->id();
        m_database->updateMetaValue("selected_id", id);
        emit noteChanged(id);
    } catch (const SqlQueryError& e) {
        qCritical() << "Error update selected_id: " << e.error();
    }
}
