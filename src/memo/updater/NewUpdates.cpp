#include "NewUpdates.h"
#include "UpdateDownloader.h"
#include <QtWidgets>

NewUpdates::NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent) : QDialog(parent){
    setWindowTitle(tr("New Updates Available"));
    resize(600, 300);

    QString description;

    for (const auto& update : updates) {
        description += tr("Version: %1 (%2) - Date: %3\n").arg(update.version, update.channel, update.date);
        description += update.description;
        description += "\n\n";
    }

    int size = updates.first().size;
    url = updates.first().url;

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
    gridLayout->addWidget(new QLabel(sizeToMegabyte(size)), 2, 1);

    layout->addLayout(gridLayout);

    progressBar = new QProgressBar;
    progressBar->setMaximum(size);
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

QString NewUpdates::updateDir() const {
    return m_updateDir;
}

void NewUpdates::reject() {
    updateDownloader->cancel();
    QDialog::reject();
}

void NewUpdates::startUpdate() {
    updateButton->setEnabled(false);
    updateDownloader->download(url);
}

void NewUpdates::finishUpdate(const QString& updateDir) {
    m_updateDir = updateDir;
    accept();
}

QString NewUpdates::sizeToMegabyte(int size) {
    return QString::number(size / 1000000.0, 'f', 1) + " " + tr("MB");
}
