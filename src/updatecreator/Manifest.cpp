#include "Manifest.h"
#include "UploadDialog.h"
#include "Constants.h"
#include <QtWidgets>

Manifest::Manifest(ProjectSettings* settings, QWidget* parent) : QWidget(parent), projectSettings(settings) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto generalGroupBox = new QGroupBox(tr("General"));

    auto generalGridLayout = new QGridLayout;
    generalGroupBox->setLayout(generalGridLayout);

    generalGridLayout->addWidget(new QLabel(tr("Path:")), 0, 0);
    manifestLabel = new QLabel;
    generalGridLayout->addWidget(manifestLabel, 0, 1);

    generalGridLayout->addWidget(new QLabel(tr("File template:")), 1, 0);
    templateLineEdit = new QLineEdit;
    generalGridLayout->addWidget(templateLineEdit, 1, 1);

    layout->addWidget(generalGroupBox);

    auto updateGroupBox = new QGroupBox(tr("Update"));

    auto updateGridLayout = new QGridLayout;
    updateGroupBox->setLayout(updateGridLayout);

    updateGridLayout->addWidget(new QLabel(tr("Version:")), 0, 0);
    versionLineEdit = new QLineEdit;
    updateGridLayout->addWidget(versionLineEdit, 0, 1);

    updateGridLayout->addWidget(new QLabel(tr("Date:")), 1, 0);
    dateLineEdit = new QLineEdit;
    updateGridLayout->addWidget(dateLineEdit, 1, 1);

    updateGridLayout->addWidget(new QLabel("Channel:"), 3, 0);
    channelComboBox = new QComboBox;
    channelComboBox->addItem(Constants::Channel::Alpha);
    channelComboBox->addItem(Constants::Channel::Beta);
    channelComboBox->addItem(Constants::Channel::Release);
    updateGridLayout->addWidget(channelComboBox, 3, 1, Qt::AlignLeft);

    updateGridLayout->addWidget(new QLabel(tr("Size:")), 4, 0);

    auto sizeLayout = new QGridLayout;
    sizeLayout->addWidget(new QLabel("Windows"), 0, 0);
    sizeLayout->addWidget(new QLabel("Linux"), 0, 1);
    sizeLayout->addWidget(new QLabel("macOS"), 0, 2);
    sizeLayout->setColumnStretch(2, 1);

    sizeWindowsLabel = new QLabel;
    sizeLayout->addWidget(sizeWindowsLabel, 1, 0);

    sizeLinuxLabel = new QLabel;
    sizeLayout->addWidget(sizeLinuxLabel, 1, 1);

    sizeMacOSLabel = new QLabel;
    sizeLayout->addWidget(sizeMacOSLabel, 1, 1);

    updateGridLayout->addLayout(sizeLayout, 4, 1);

    updateGridLayout->addWidget(new QLabel(tr("Description:")), 5, 0);
    descriptionTextEdit = new QPlainTextEdit;
    updateGridLayout->addWidget(descriptionTextEdit, 5, 1);

    updateGridLayout->setRowStretch(2, 0);

    layout->addWidget(updateGroupBox);
    layout->setStretch(1, 0);

    auto uploadButton = new QPushButton(tr("Upload..."));
    connect(uploadButton, &QPushButton::clicked, this, &Manifest::upload);
    layout->addWidget(uploadButton, 0, Qt::AlignLeft);

    connect(qApp, &QApplication::focusChanged, this, &Manifest::onFocusChanged);
}

void Manifest::populateUpdate(const ListModel::Update& update) {
    versionLineEdit->setText(update.version);
    dateLineEdit->setText(update.date);

    int channelIndex = channelComboBox->findText(update.channel);
    channelComboBox->setCurrentIndex(channelIndex);

    sizeWindowsLabel->setText(update.size.contains(Constants::OS::Windows) ? QString::number(update.size[Constants::OS::Windows]) : "");
    sizeLinuxLabel->setText(update.size.contains(Constants::OS::Linux) ? QString::number(update.size[Constants::OS::Linux]) : "");
    sizeMacOSLabel->setText(update.size.contains(Constants::OS::MacOS) ? QString::number(update.size[Constants::OS::MacOS]) : "");

    descriptionTextEdit->setPlainText(update.description);
}

ListModel::Update Manifest::getUpdate() const {
    ListModel::Update update;
    update.version = versionLineEdit->text();
    update.date = dateLineEdit->text();
    update.channel = channelComboBox->currentText();

    if (!sizeWindowsLabel->text().isEmpty()) {
        update.size[Constants::OS::Windows] = sizeWindowsLabel->text().toInt();
    }

    if (!sizeLinuxLabel->text().isEmpty()) {
        update.size[Constants::OS::Linux] = sizeLinuxLabel->text().toInt();
    }

    if (!sizeMacOSLabel->text().isEmpty()) {
        update.size[Constants::OS::MacOS] = sizeMacOSLabel->text().toInt();
    }

    update.description = descriptionTextEdit->document()->toPlainText();

    return update;
}

void Manifest::setManifestPath(const QString& path) {
    manifestLabel->setText(path);
}

void Manifest::setFileTemplate(const QString& fileTemplate) {
    templateLineEdit->setText(fileTemplate);
}

QString Manifest::getFileTemplate() const {
    return templateLineEdit->text();
}

void Manifest::setFileSize(const QString& os, qint64 size) {
    QString value = QString::number(size);

    if (os == Constants::OS::Windows) {
        sizeWindowsLabel->setText(value);
    } else if (os == Constants::OS::Linux) {
        sizeLinuxLabel->setText(value);
    } else if (os == Constants::OS::MacOS) {
        sizeMacOSLabel->setText(value);
    }
}

QString Manifest::getVersion() {
    return versionLineEdit->text();
}

void Manifest::clear() {
    templateLineEdit->clear();
    clearUpdate();
}

void Manifest::clearUpdate() {
    versionLineEdit->clear();
    dateLineEdit->clear();
    channelComboBox->setCurrentIndex(0);
    sizeLinuxLabel->clear();
    sizeWindowsLabel->clear();
    sizeMacOSLabel->clear();
    descriptionTextEdit->clear();
}

void Manifest::onFocusChanged(QWidget* from, QWidget* to) {
    Q_UNUSED(to)

    QWidget* rootTo = root(to);
    QWidget* rootFrom = root(from);

    if (rootFrom == this && rootTo != this) {
        emit lostFocus();
    }
}

void Manifest::upload() {
    UploadDialog uploadDialog(UploadDialog::FileType::Manifest, manifestLabel->text(), projectSettings);
    uploadDialog.exec();
}

QWidget* Manifest::root(QWidget* child) {
    auto widget = child;

    while (widget != nullptr && widget != this) {
         widget = widget->parentWidget();
    }

    return widget;
}
