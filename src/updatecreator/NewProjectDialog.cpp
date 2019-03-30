#include "NewProjectDialog.h"
#include "Constants.h"
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
    if (createProject()) {
        QDialog::accept();
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

bool NewProjectDialog::createProject() {
    QString workspaceDir = workspaceLineEdit->text();
    if (workspaceDir.isEmpty() || !QDir(workspaceDir).exists()) {
        QMessageBox::critical(this, tr("Error"), tr("Wrong workspace directory"));
        return false;
    }

    QString appDir = appLineEdit->text();
    if (appDir.isEmpty() || !QDir(appDir).exists()) {
        QMessageBox::critical(this, tr("Error"), tr("Wrong application directory"));
        return false;
    }

    QFile file(workspaceDir + "/" + Constants::ProjectName);

    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open project file for writting"));
        return false;
    }

    QJsonObject project;
    project["applicationDir"] = appDir;

    file.write(QJsonDocument(project).toJson());

    return true;
}
