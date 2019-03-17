#include "NewUpdate.h"
#include <QtWidgets>

NewUpdate::NewUpdate(const UpdateChecker::Update& update, int size, QWidget* parent) : QDialog(parent){
    setWindowTitle(tr("New Update Available"));
    resize(600, 300);

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    QGridLayout* gridLayout = new QGridLayout;

    gridLayout->addWidget(new QLabel(tr("Version:")), 0, 0);
    gridLayout->addWidget(new QLabel(update.version), 0, 1);

    gridLayout->addWidget(new QLabel(tr("Date:")), 1, 0);
    gridLayout->addWidget(new QLabel(update.date), 1, 1);

    gridLayout->addWidget(new QLabel(tr("Size:")), 2, 0);
    QString updateSize = QString::number(size / 1000000) + " " + tr("MB");
    gridLayout->addWidget(new QLabel(updateSize), 2, 1);

    gridLayout->addWidget(new QLabel(tr("Channel:")), 3, 0);
    gridLayout->addWidget(new QLabel(update.channel), 3, 1);

    gridLayout->addWidget(new QLabel(tr("Description:")), 4, 0);
    QPlainTextEdit* textEdit = new QPlainTextEdit(update.description);
    textEdit->setReadOnly(true);
    gridLayout->addWidget(textEdit, 4, 1);

    layout->addLayout(gridLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    buttonBox->addButton(tr("Update"), QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
