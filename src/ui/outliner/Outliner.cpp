#include "Outliner.h"
#include "TreeModel.h"
#include <QtWidgets>

Outliner::Outliner() {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &Outliner::onCustomContextMenu);
    createContextMenu();

    TreeModel* model = new TreeModel(this);
    setModel(model);
}

void Outliner::onCustomContextMenu(const QPoint& point) {
    if (indexAt(point).isValid()) {
        contextMenu->exec(mapToGlobal(point));
    } else {
        contextMenu->exec(mapToGlobal(point));
    }
}

void Outliner::addNote() {
    qDebug() << "add note";
}

void Outliner::createContextMenu() {
    contextMenu = new QMenu(this);

    contextMenu->addAction(tr("Add..."), this, &Outliner::addNote);
}
