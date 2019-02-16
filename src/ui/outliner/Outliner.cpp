#include "Outliner.h"
#include "TreeModel.h"
#include <QtWidgets>

Outliner::Outliner() {
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

void Outliner::onCustomContextMenu(const QPoint& point) {
    if (indexAt(point).isValid()) {
        contextMenu->exec(mapToGlobal(point));
    } else {
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

void Outliner::createContextMenu() {
    contextMenu = new QMenu(this);
    contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
}

void Outliner::insertChild(const QString& title) {
    QModelIndex index = selectionModel()->currentIndex();

    if (model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index)) {
            return;
        }
    }

    if (!model->insertRow(0, index)) {
        return;
    }

    QModelIndex child = model->index(0, 0, index);
    model->setData(child, QVariant(title), Qt::EditRole);

    selectionModel()->setCurrentIndex(model->index(0, 0, index), QItemSelectionModel::ClearAndSelect);
    updateActions();
}
