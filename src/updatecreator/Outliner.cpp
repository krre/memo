#include "Outliner.h"
#include <QtWidgets>

Outliner::Outliner(QWidget* parent) : QWidget(parent) {
    setMinimumWidth(100);

    auto layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    setLayout(layout);

    listWidget = new QListWidget;
    layout->addWidget(listWidget);

    auto addButton = new QPushButton(tr("Add"));
    connect(addButton, &QPushButton::clicked, this, &Outliner::addUpdate);

    auto removeButton = new QPushButton(tr("Remove"));
    connect(removeButton, &QPushButton::clicked, this, &Outliner::removeUpdate);

    auto buttonLaoyut = new QHBoxLayout;
    buttonLaoyut->setMargin(5);
    buttonLaoyut->setSpacing(5);
    buttonLaoyut->addWidget(addButton);
    buttonLaoyut->addWidget(removeButton);

    layout->addLayout(buttonLaoyut);
}

void Outliner::addUpdate() {
    qDebug() << "add";
}

void Outliner::removeUpdate() {
    qDebug() << "remove";
}
