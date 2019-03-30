#include "NewProjectDialog.h"
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

void NewProjectDialog::accept() {
    QDialog::accept();
}

void NewProjectDialog::selectAppDir() {

}

void NewProjectDialog::selectWorkspaceDir() {

}
