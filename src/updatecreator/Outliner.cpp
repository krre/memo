#include "Outliner.h"
#include "ListModel.h"
#include <QtWidgets>

Outliner::Outliner(ListModel* model, QWidget* parent) : QWidget(parent) {
    setMinimumWidth(100);

    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    listView = new QListView;
    listView->setModel(model);
    layout->addWidget(listView);

    auto addButton = new QPushButton(tr("Add"));
    connect(addButton, &QPushButton::clicked, this, &Outliner::addClicked);

    auto removeButton = new QPushButton(tr("Remove"));
    connect(removeButton, &QPushButton::clicked, this, &Outliner::removeUpdate);

    auto buttonLaoyut = new QHBoxLayout;
    buttonLaoyut->setMargin(5);
    buttonLaoyut->setSpacing(5);
    buttonLaoyut->addWidget(addButton);
    buttonLaoyut->addWidget(removeButton);

    layout->addLayout(buttonLaoyut);
}

void Outliner::selectRow(int row) {
    listView->setCurrentIndex(listView->model()->index(row, 0));
}

void Outliner::removeUpdate() {
    removeClicked(listView->currentIndex().row());
}
