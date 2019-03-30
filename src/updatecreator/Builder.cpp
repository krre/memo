#include "Builder.h"
#include <QtWidgets>

Builder::Builder(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    createAppDirWidgets();
    createFilesWidgets();

    auto buildButton = new QPushButton(tr("Build"));
    layout->addWidget(buildButton, 1, Qt::AlignLeft);

    readSettings();
}

Builder::~Builder() {
    writeSettings();
}

void Builder::selectDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        appDirLineEdit->setText(directory);
    }
}

void Builder::createAppDirWidgets() {
    auto appDirLayout = new QHBoxLayout;
    appDirLayout->addWidget(new QLabel(tr("Application:")));
    appDirLineEdit = new QLineEdit;
    appDirLayout->addWidget(appDirLineEdit);
    auto appDirButton = new QPushButton(tr("Browse..."));
    connect(appDirButton, &QPushButton::clicked, this, &Builder::selectDirectory);
    appDirLayout->addWidget(appDirButton);

    auto mainLayout = static_cast<QVBoxLayout*>(layout());
    mainLayout->addLayout(appDirLayout);
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
