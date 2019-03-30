#include "Builder.h"
#include <QtWidgets>

Builder::Builder(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    createFilesWidgets();

    auto buildButton = new QPushButton(tr("Build"));
    layout->addWidget(buildButton, 1, Qt::AlignLeft);

    readSettings();
}

Builder::~Builder() {
    writeSettings();
}

void Builder::createFilesWidgets() {
    auto filesGroupBox = new QGroupBox(tr("Files"));

    auto filesLayout = new QVBoxLayout;
    filesGroupBox->setLayout(filesLayout);

    auto splitter = new QSplitter;
    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);

    splitter->addWidget(new QListWidget);
    splitter->addWidget(new QListWidget);

    filesLayout->addWidget(splitter);

    auto mainLayout = static_cast<QVBoxLayout*>(layout());
    mainLayout->addWidget(filesGroupBox, 1);
}

void Builder::readSettings() {
    QSettings settings;
}

void Builder::writeSettings() {
    QSettings settings;
}
