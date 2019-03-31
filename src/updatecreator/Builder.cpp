#include "Builder.h"
#include "ProjectSettings.h"
#include <QtWidgets>

Builder::Builder(ProjectSettings* settings, QWidget* parent) : QWidget(parent), projectSettings(settings) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    createAppDirWidgets();
    createFilesWidgets();

    auto buildButton = new QPushButton(tr("Build"));
    layout->addWidget(buildButton, 1, Qt::AlignLeft);
}

void Builder::load() {
    appDirLineEdit->setText(projectSettings->appDir());
}

void Builder::clear() {
    appDirLineEdit->clear();
}

void Builder::selectDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        appDirLineEdit->setText(directory);
        projectSettings->setAppDir(directory);
    }
}

void Builder::createAppDirWidgets() {
    auto appDirLayout = new QHBoxLayout;
    appDirLayout->addWidget(new QLabel(tr("Application:")));
    appDirLineEdit = new QLineEdit;
    connect(appDirLineEdit, &QLineEdit::textEdited, [this] (const QString& text) {
        projectSettings->setAppDir(text);
    });
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
