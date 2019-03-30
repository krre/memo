#include "NewProjectDialog.h"
#include "Constants.h"
#include "lib/Exception.h"
#include <QtWidgets>

NewProjectDialog::NewProjectDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("New Project"));

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto workspaceLayout = new QHBoxLayout;
    workspaceLayout->addWidget(new QLabel(tr("Workspace:")));
    workspaceLineEdit = new QLineEdit;
    workspaceLayout->addWidget(workspaceLineEdit);
    auto workspaceButton = new QPushButton(tr("Browse..."));
    connect(workspaceButton, &QPushButton::clicked, this, &NewProjectDialog::selectWorkspaceDir);
    workspaceLayout->addWidget(workspaceButton);

    layout->addLayout(workspaceLayout);

    auto appLayout = new QHBoxLayout;
    appLayout->addWidget(new QLabel(tr("Application:")));
    appLineEdit = new QLineEdit;
    appLayout->addWidget(appLineEdit);
    auto appButton = new QPushButton(tr("Browse..."));
    connect(appButton, &QPushButton::clicked, this, &NewProjectDialog::selectAppDir);
    appLayout->addWidget(appButton);

    layout->addLayout(appLayout);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    adjustSize();
    resize(500, height());
}

QString NewProjectDialog::workspaceDir() const {
    return workspaceLineEdit->text();
}

void NewProjectDialog::accept() {
    try {
        createProject();
        QDialog::accept();
    } catch (const MemoLib::RuntimeError& e) {
        QMessageBox::critical(this, tr("Error"), e.text());
    }
}

void NewProjectDialog::selectAppDir() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        appLineEdit->setText(directory);
    }
}

void NewProjectDialog::selectWorkspaceDir() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        workspaceLineEdit->setText(directory);
    }
}

void NewProjectDialog::createProject() {
    QString workspaceDir = workspaceLineEdit->text();
    if (workspaceDir.isEmpty() || !QDir(workspaceDir).exists()) {
        throw MemoLib::RuntimeError(tr("Wrong workspace directory"));
    }

    QString appDir = appLineEdit->text();
    if (appDir.isEmpty() || !QDir(appDir).exists()) {
        throw MemoLib::RuntimeError(tr("Wrong application directory"));
    }

    QFile file(workspaceDir + "/" + Constants::ProjectName);

    if (!file.open(QIODevice::WriteOnly)) {
        throw MemoLib::RuntimeError(tr("Failed to open project file for writting"));
    }

    QJsonObject project;
    project["applicationDir"] = appDir;

    file.write(QJsonDocument(project).toJson());
}
