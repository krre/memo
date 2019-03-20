#include "NewUpdates.h"
#include "UpdateDownloader.h"
#include <QtWidgets>

NewUpdates::NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent) : QDialog(parent){
    setWindowTitle(tr("New Updates Available"));
    resize(600, 300);

    QString description;
    int totalSize = 0;
    int i = updates.count() - 1;

    for (const auto& update : updates) {
        description += tr("Version: %1 - Date: %2 - Size: %3.\n").arg(update.version, update.date, sizeToMegabyte(update.size));
        description += update.description;
        description += "\n\n";

        totalSize += update.size;

        // In reverse order.
        urls.append(updates.at(i--).url);
    }

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto textEdit = new QPlainTextEdit(description);
    textEdit->setReadOnly(true);
    layout->addWidget(textEdit);

    auto gridLayout = new QGridLayout;
    gridLayout->setColumnStretch(1, 1);

    gridLayout->addWidget(new QLabel(tr("Channel:")), 0, 0);
    gridLayout->addWidget(new QLabel(updates.first().channel), 0, 1);

    gridLayout->addWidget(new QLabel(tr("Count:")), 1, 0);
    gridLayout->addWidget(new QLabel(QString::number(updates.count())), 1, 1);

    gridLayout->addWidget(new QLabel(tr("Size:")), 2, 0);
    gridLayout->addWidget(new QLabel(sizeToMegabyte(totalSize)), 2, 1);

    layout->addLayout(gridLayout);

    progressBar = new QProgressBar;
    progressBar->setMaximum(totalSize);
    layout->addWidget(progressBar);

    auto buttonBox = new QDialogButtonBox;
    updateButton = new QPushButton(tr("Update"));
    buttonBox->addButton(updateButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewUpdates::startUpdate);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    updateDownloader = new UpdateDownloader(this);
}

void NewUpdates::startUpdate() {
    updateButton->setEnabled(false);
    updateDownloader->download(urls);
}

QString NewUpdates::sizeToMegabyte(int size) {
    return QString::number(size / 1000000.0, 'f', 1) + " " + tr("MB");
}
