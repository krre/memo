#include "Tasks.h"
#include <QToolBar>
#include <QTableWidget>
#include <QVBoxLayout>

Tasks::Tasks(Database* database, Settings* settings) : StandardWindow(settings), m_database(database) {
    setWindowTitle(tr("Tasks"));

    auto toolBar = new QToolBar;

    auto addAction = toolBar->addAction(tr("Add"));
    connect(addAction, &QAction::triggered, this, &Tasks::add);

    auto editAction = toolBar->addAction(tr("Edit"));
    connect(editAction, &QAction::triggered, this, &Tasks::edit);

    auto removeAction = toolBar->addAction(tr("Remove"));
    connect(removeAction, &QAction::triggered, this, &Tasks::remove);

    auto tagsAction = toolBar->addAction(tr("Tags"));
    connect(tagsAction, &QAction::triggered, this, &Tasks::openTags);

    auto table = new QTableWidget;

    auto verticalLayout = new QVBoxLayout;
    setLayout(verticalLayout);

    auto margins = verticalLayout->contentsMargins();
    verticalLayout->setContentsMargins(margins.left(), 0, margins.right(), margins.bottom());

    verticalLayout->addWidget(toolBar);
    verticalLayout->addWidget(table);
}

QString Tasks::settingsGroup() const {
    return "Tasks";
}

void Tasks::add() {

}

void Tasks::edit() {

}

void Tasks::remove() {

}

void Tasks::openTags() {

}
