#include "Outliner.h"
#include "TreeModel.h"
#include "../../database/Database.h"
#include <QtWidgets>

Outliner::Outliner(Database* database) : database(database) {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &Outliner::onCustomContextMenu);
    createContextMenu();
    header()->setVisible(false);

    model = new TreeModel(this);
    setModel(model);
}

void Outliner::updateActions() {
    bool hasCurrent = selectionModel()->currentIndex().isValid();

    if (hasCurrent) {
        closePersistentEditor(selectionModel()->currentIndex());
    }
}

void Outliner::build() {

}

void Outliner::onCustomContextMenu(const QPoint& point) {
    updateContextMenu();
    contextMenu->exec(mapToGlobal(point));
}

void Outliner::addNote() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Add Note"), tr("Name:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        insertChild(text);
    }
}

void Outliner::removeNote() {
    QModelIndex index = selectionModel()->currentIndex();
    int result = QMessageBox::question(this, tr("Remove Note"), tr("Remove %1?").arg(model->data(index).toString()));
    if (result == QMessageBox::Yes) {
        model->removeRow(index.row(), index.parent());
    }
}

void Outliner::renameNote() {
    edit(selectionModel()->currentIndex());
}

void Outliner::createContextMenu() {
    contextMenu = new QMenu(this);
    contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
    removeAction = contextMenu->addAction(tr("Remove..."), this, &Outliner::removeNote);
    renameAction = contextMenu->addAction(tr("Rename"), this, &Outliner::renameNote);
}

void Outliner::updateContextMenu() {
    bool enabled = currentIndex().isValid();
    removeAction->setEnabled(enabled);
    renameAction->setEnabled(enabled);
}

void Outliner::insertChild(const QString& title) {
    QModelIndex index = selectionModel()->currentIndex();

    if (!model->insertRow(0, index)) {
        return;
    }

    QModelIndex child = model->index(0, 0, index);
    model->setData(child, QVariant(title), Qt::EditRole);

    selectionModel()->setCurrentIndex(model->index(0, 0, index), QItemSelectionModel::ClearAndSelect);
    updateActions();

    int id = database->insertRecord(index.row() + 1, 0, 0, title);

    emit noteAdded(id);
}


void Outliner::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !indexAt(event->localPos().toPoint()).isValid()) {
        selectionModel()->clearSelection();
        setCurrentIndex(QModelIndex());
        event->accept();
    } else {
        QTreeView::mousePressEvent(event);
    }
}
