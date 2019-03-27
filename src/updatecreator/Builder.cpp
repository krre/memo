#include "Builder.h"
#include <QtWidgets>

Builder::Builder(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    createDirectoryWidgets();
    createFilesWidgets();

    auto buildButton = new QPushButton(tr("Build"));
    layout->addWidget(buildButton, 1, Qt::AlignLeft);

    readSettings();
}

Builder::~Builder() {
    writeSettings();
}

void Builder::selectAppDir() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        appLineEdit->setText(directory);
    }
}

void Builder::selectWorkspaceDir() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        workspaceLineEdit->setText(directory);
    }
}

void Builder::createDirectoryWidgets() {
    auto directoriesGroupBox = new QGroupBox(tr("Directories"));

    auto directoriesLayout = new QVBoxLayout;

    auto appLayout = new QHBoxLayout;
    appLayout->addWidget(new QLabel(tr("Application:")));
    appLineEdit = new QLineEdit;
    appLayout->addWidget(appLineEdit);
    auto appButton = new QPushButton(tr("Browse..."));
    connect(appButton, &QPushButton::clicked, this, &Builder::selectAppDir);
    appLayout->addWidget(appButton);

    directoriesLayout->addLayout(appLayout);

    directoriesGroupBox->setLayout(directoriesLayout);

    auto workspaceLayout = new QHBoxLayout;
    workspaceLayout->addWidget(new QLabel(tr("Workspace:")));
    workspaceLineEdit = new QLineEdit;
    workspaceLayout->addWidget(workspaceLineEdit);
    auto workspaceButton = new QPushButton(tr("Browse..."));
    connect(workspaceButton, &QPushButton::clicked, this, &Builder::selectWorkspaceDir);
    workspaceLayout->addWidget(workspaceButton);

    directoriesLayout->addLayout(workspaceLayout);

    layout()->addWidget(directoriesGroupBox);
}

void Builder::createFilesWidgets() {
    auto filesGroupBox = new QGroupBox(tr("Files"));

    auto filesLayout = new QVBoxLayout;
    filesGroupBox->setLayout(filesLayout);

    auto createButton = new QPushButton(tr("Create"));
    filesLayout->addWidget(createButton, 1, Qt::AlignLeft);

    layout()->addWidget(filesGroupBox);
}

void Builder::readSettings() {
    QSettings settings;
    appLineEdit->setText(settings.value("Builder/appDir").toString());
    workspaceLineEdit->setText(settings.value("Builder/workspaceDir").toString());
}

void Builder::writeSettings() {
    QSettings settings;
    settings.setValue("Builder/appDir", appLineEdit->text());
    settings.setValue("Builder/workspaceDir", workspaceLineEdit->text());
}
