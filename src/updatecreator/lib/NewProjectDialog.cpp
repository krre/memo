#include "NewProjectDialog.h"
#include <common/Exception.h>
#include <QtWidgets>

NewProjectDialog::NewProjectDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle(tr("New Project"));

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto gridLayout = new QGridLayout;

    gridLayout->addWidget(new QLabel(tr("Name:")), 0, 0);
    nameLineEdit = new QLineEdit;
    gridLayout->addWidget(nameLineEdit, 0, 1);

    gridLayout->addWidget(new QLabel(tr("Directory:")), 1, 0);
    directoryLineEdit = new QLineEdit;
    auto directoryLayout = new QHBoxLayout;
    directoryLayout->addWidget(directoryLineEdit);
    auto directoryButton = new QPushButton(tr("Browse..."));
    connect(directoryButton, &QPushButton::clicked, this, &NewProjectDialog::selectDirectory);
    directoryLayout->addWidget(directoryButton);

    gridLayout->addLayout(directoryLayout, 1, 1);

    layout->addLayout(gridLayout);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    adjustSize();
    resize(500, height());
}

QString NewProjectDialog::projectDir() const {
    return projectDirPath;
}

void NewProjectDialog::accept() {
    try {
        QString name = nameLineEdit->text();

        if (name.isEmpty()) {
            throw Common::RuntimeError(tr("Name is empty"));
        }

        QString directory = directoryLineEdit->text();

        if (directory.isEmpty() || !QDir(directory).exists()) {
            throw Common::RuntimeError(tr("Wrong directory path"));
        }

        projectDirPath = directory + "/" + name;

        QDir dir;

        if (!dir.mkpath(projectDirPath)) {
            throw Common::RuntimeError(tr("Failed to create project directory"));
        }

        QDialog::accept();
    } catch (const Common::RuntimeError& e) {
        QMessageBox::critical(this, tr("Error"), e.error());
    }
}

void NewProjectDialog::selectDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        directoryLineEdit->setText(directory);
    }
}
