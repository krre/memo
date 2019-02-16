#include "Outliner.h"
#include <QtWidgets>

Outliner::Outliner() {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTreeView::customContextMenuRequested, this, &Outliner::onCustomContextMenu);
    createContextMenu();
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

    QAction* addAction = contextMenu->addAction(tr("Add"));
    connect(addAction, &QAction::triggered, this, &Outliner::addNote);
}
