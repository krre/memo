#include "NoteTaking.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "NoteProperties.h"
#include "core/Application.h"
#include "database/Database.h"
#include "database/DatabaseException.h"
#include <QHeaderView>
#include <QMenu>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QMouseEvent>

NoteTaking::NoteTaking(Database* database) : m_database(database) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &NoteTaking::onCustomContextMenu);
    header()->setVisible(false);

    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::SingleSelection);

    clear();

    connect(itemDelegate(), &QAbstractItemDelegate::closeEditor, this, [=, this] {
        auto item = m_model->item(selectionModel()->currentIndex());
        database->updateNoteValue(item->id(), "title", item->data());
    });
}

void NoteTaking::build() {
    clear();
    QVector<Note> notes = m_database->notes();

    for (const Note& note : std::as_const(notes)) {
        auto parentItem = m_model->root()->find(note.parentId);
        QModelIndex parentIndex = m_model->index(parentItem);
        m_model->insertRow(note.pos, parentIndex);

        QModelIndex index = m_model->index(note.pos, 0, parentIndex);
        m_model->setData(index, note.title, Qt::EditRole);
        m_model->item(index)->setId(note.id);
    }

    int selectedId = m_database->metaValue("selected_id").toInt();

    if (selectedId == 0) {
        setCurrentIndex(QModelIndex());
    } else {
        setCurrentId(selectedId);
    }
}

void NoteTaking::clear() {
    m_isInited = false;
    m_model.reset(new TreeModel);
    setModel(m_model.data());
    connect(m_model.data(), &TreeModel::itemDropped, this, &NoteTaking::moveTree);

    m_isInited = true;
}

void NoteTaking::onCustomContextMenu(const QPoint& point) {
    if (!m_database->isOpen()) return;

    auto contextMenu = new QMenu(this);
    contextMenu->setAttribute(Qt::WA_DeleteOnClose);
    contextMenu->addAction(tr("Add..."), this, &NoteTaking::addNote);
    auto removeAction = contextMenu->addAction(tr("Remove..."), this, &NoteTaking::removeNotes);
    auto renameAction = contextMenu->addAction(tr("Rename"), this, &NoteTaking::renameNote);

    contextMenu->addSeparator();

    auto moveUpAction = contextMenu->addAction(tr("Move Up"), this, &NoteTaking::moveUp);
    auto moveDownAction = contextMenu->addAction(tr("Move Down"), this, &NoteTaking::moveDown);

    contextMenu->addSeparator();

    auto expandAction = contextMenu->addAction(tr("Expand"), this, &NoteTaking::expandTree);
    contextMenu->addAction(tr("Collapse All"), this, &NoteTaking::collapseAll);
    contextMenu->addAction(tr("Expand All"), this, &NoteTaking::expandAll);

    contextMenu->addSeparator();

    auto propertiesAction = contextMenu->addAction(tr("Properties..."), this, &NoteTaking::showProperties);

    bool enabled = currentIndex().isValid();
    removeAction->setEnabled(enabled);
    renameAction->setEnabled(enabled);
    moveUpAction->setEnabled(enabled && currentIndex().row() > 0);
    moveDownAction->setEnabled(enabled && currentIndex().row() < m_model->rowCount(currentIndex().parent()) - 1);
    expandAction->setEnabled(enabled);
    propertiesAction->setEnabled(enabled);

    contextMenu->exec(mapToGlobal(point));
}

void NoteTaking::addNote() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Note"), tr("Name:"), QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
        insertChild(text);
    }
}

void NoteTaking::removeNotes() {
    QModelIndex index = selectionModel()->currentIndex();

    if (QMessageBox::question(this, Application::Name,
                              tr("Remove %1?").arg(m_model->data(index).toString())) == QMessageBox::No) return;

    auto item = m_model->item(index);
    Ids ids = m_model->childIds(item);

    m_model->removeRow(index.row(), index.parent());

    auto parentItem = m_model->item(index.parent());

    for (int i = 0; i < parentItem->childCount(); ++i) {
        Id id = parentItem->child(i)->id();
        m_database->updateNoteValue(id, "pos", i);
    }

    for (Id id : std::as_const(ids)) {
        m_database->removeNote(id);
    }
}

void NoteTaking::renameNote() {
    edit(selectionModel()->currentIndex());
}

void NoteTaking::moveUp() {
    int row = currentIndex().row();

    Id id1 = m_model->item(currentIndex())->id();
    Id id2 = m_model->item(currentIndex().sibling(row - 1, 0))->id();

    m_model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row - 1);

    m_database->updateNoteValue(id1, "pos", row - 1);
    m_database->updateNoteValue(id2, "pos", row);
}

void NoteTaking::moveDown() {
    int row = currentIndex().row();

    Id id1 = m_model->item(currentIndex())->id();
    Id id2 = m_model->item(currentIndex().sibling(row + 1, 0))->id();

    m_model->moveRow(currentIndex().parent(), row, currentIndex().parent(), row + 2);

    m_database->updateNoteValue(id1, "pos", row + 1);
    m_database->updateNoteValue(id2, "pos", row);
}

void NoteTaking::moveTree(const QModelIndex& index) {
    selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
    expand(index.parent());

    // Move tree in database.
    auto targetItem = m_model->item(index);
    Id sourceId = targetItem->id();
    Id sourceParentId = m_database->noteValue(sourceId, "parent_id").toInt();
    Id destinationParentId = targetItem->parent()->id();

    auto sourceParentItem = m_model->root()->find(sourceParentId);

    // Rewrite note positions on source parent.
    for (int i = 0; i < sourceParentItem->childCount(); ++i) {
        m_database->updateNoteValue(sourceParentItem->child(i)->id(), "pos", i);
    }

    if (sourceParentId == destinationParentId) return;

    m_database->updateNoteValue(sourceId, "parent_id", destinationParentId.toVariant());

    auto destinationParentItem = m_model->root()->find(destinationParentId);

     // Rewrite note positions on destination parent.
    for (int i = 0; i < destinationParentItem->childCount(); ++i) {
        m_database->updateNoteValue(destinationParentItem->child(i)->id(), "pos", i);
    }

     // Rewrite depth in all children of target note.
    Ids childIds = m_model->childIds(targetItem);

    for (Id id : std::as_const(childIds)) {
        int depth = targetItem->find(id)->depth() - 1;
        m_database->updateNoteValue(id, "depth", depth);
    }
}

void NoteTaking::expandTree() {
    expandRecursively(currentIndex());
}

void NoteTaking::showProperties() const {
    Id id = m_model->item(currentIndex())->id();
    Note note = m_database->note(id);

    NoteProperties props(note);
    props.exec();
}

void NoteTaking::insertChild(const QString& title) {
    QModelIndex currentIndex = selectionModel()->currentIndex();
    auto currentItem = m_model->item(currentIndex);

    Id currentId = currentItem->id();
    int pos = currentItem->childCount();
    int depth = currentItem->depth();
    Id noteId = m_database->insertNote(currentId, pos, depth, title);

    if (!m_model->insertRow(pos, currentIndex)) {
        return;
    }

    QModelIndex noteIndex = m_model->index(pos, 0, currentIndex);
    m_model->setData(noteIndex, title, Qt::EditRole);
    m_model->item(noteIndex)->setId(noteId);

    selectionModel()->setCurrentIndex(m_model->index(pos, 0, currentIndex), QItemSelectionModel::ClearAndSelect);
    setExpanded(currentIndex, true);
}

int NoteTaking::exportNote(Id parentId, const QString& path) const {
    QDir().mkpath(path);

    int count = 0;
    auto parentItem = m_model->root()->find(parentId);

    for (int i = 0; i < parentItem->childCount(); ++i) {
        auto childItem = parentItem->child(i);
        QString title = childItem->data().toString();
        QString note = m_database->noteValue(childItem->id(), "note").toString();
        QString notePath = path + "/" + title;

        QString filename = notePath + ".txt";
        QFile file(filename);

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
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

void NoteTaking::setCurrentId(Id id) {
    auto item = m_model->root()->find(id);
    QModelIndex index = m_model->index(item);
    setCurrentIndex(index);

    auto parent = item->parent();

    while (parent != m_model->root()) {
        setExpanded(m_model->index(parent), true);
        parent = parent->parent();
    }
}

void NoteTaking::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !indexAt(event->position().toPoint()).isValid()) {
        selectionModel()->clearSelection();
        setCurrentIndex(QModelIndex());
    } else {
        QTreeView::mousePressEvent(event);
    }
}

void NoteTaking::currentChanged(const QModelIndex& current, const QModelIndex& previous [[maybe_unused]]) {
    if (!m_isInited) return;

    try {
        Id id = m_model->item(current)->id();
        m_database->updateMetaValue("selected_id", id.toVariant());
        emit noteChanged(id);
    } catch (const SqlQueryError& e) {
        qCritical() << "Error update selected_id: " << e.error();
    }
}
