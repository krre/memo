#include "NewUpdates.h"
#include <QtWidgets>

NewUpdates::NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent) : QDialog(parent){
    setWindowTitle(tr("New Updates Available"));
    resize(600, 300);

    QVBoxLayout* layout = new QVBoxLayout;
    setLayout(layout);

    QString description;

    QPlainTextEdit* textEdit = new QPlainTextEdit(description);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(1, 1);

    gridLayout->addWidget(new QLabel(tr("Channel:")), 0, 0);
    gridLayout->addWidget(new QLabel(updates.first().channel), 0, 1);

    gridLayout->addWidget(new QLabel(tr("Count:")), 1, 0);
    gridLayout->addWidget(new QLabel(QString::number(updates.count())), 1, 1);

    gridLayout->addWidget(new QLabel(tr("Size:")), 2, 0);

    int size = 0;
    for (const auto& update : updates) {
        size += update.size;
    }

    gridLayout->addWidget(new QLabel(QString::number(size / 1000000) + " " + tr("MB")), 2, 1);

    layout->addLayout(gridLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox;
    buttonBox->addButton(tr("Update"), QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
