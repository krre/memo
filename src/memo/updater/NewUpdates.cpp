#include "NewUpdates.h"
#include "UpdateDownloader.h"
#include <QtWidgets>

NewUpdates::NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent) : QDialog(parent){
    setWindowTitle(tr("New Updates Available"));
    resize(600, 300);

    QString description;
    int totalSize = 0;

    for (const auto& update : updates) {
        description += tr("Version: %1 - Date: %2 - Size: %3.\n").arg(update.version, update.date, sizeToMegabyte(update.size));
        description += update.description;
        description += "\n\n";

        totalSize += update.size;

        urls.append(update.url);
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

    updateDownloader = new UpdateDownloader(this);
    connect(updateDownloader, &UpdateDownloader::downloadProgress, progressBar, &QProgressBar::setValue);
    connect(updateDownloader, &UpdateDownloader::finished, this, &NewUpdates::finishUpdate);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewUpdates::startUpdate);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void NewUpdates::reject() {
    updateDownloader->cancel();
    QDialog::reject();
}

void NewUpdates::startUpdate() {
    updateButton->setEnabled(false);
    updateDownloader->download(urls);
}

void NewUpdates::finishUpdate(const QString& updateDir) {
    qDebug() << updateDir;
    accept();
}

QString NewUpdates::sizeToMegabyte(int size) {
    return QString::number(size / 1000000.0, 'f', 1) + " " + tr("MB");
}
