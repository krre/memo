#include "Builder.h"
#include <QtWidgets>

Builder::Builder(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto appLayout = new QHBoxLayout;
    appLayout->addWidget(new QLabel(tr("Application:")));
    appLineEdit = new QLineEdit;
    appLayout->addWidget(appLineEdit);
    auto appButton = new QPushButton(tr("Browse..."));
    connect(appButton, &QPushButton::clicked, this, &Builder::selectAppDir);
    appLayout->addWidget(appButton);
    layout->addLayout(appLayout);

    auto workspaceLayout = new QHBoxLayout;
    workspaceLayout->addWidget(new QLabel(tr("Workspace:")));
    workspaceLineEdit = new QLineEdit;
    workspaceLayout->addWidget(workspaceLineEdit);
    auto workspaceButton = new QPushButton(tr("Browse..."));
    connect(workspaceButton, &QPushButton::clicked, this, &Builder::selectWorkspaceDir);
    workspaceLayout->addWidget(workspaceButton);
    layout->addLayout(workspaceLayout);

    auto cacheGroupBox = new QGroupBox(tr("Cache"));

    auto cacheLayout = new QVBoxLayout;
    cacheGroupBox->setLayout(cacheLayout);

    auto createButton = new QPushButton(tr("Create"));
    cacheLayout->addWidget(createButton, 1, Qt::AlignLeft);

    layout->addWidget(cacheGroupBox);

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
