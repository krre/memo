#include "Form.h"
#include "Constants.h"
#include "ProjectSettings.h"
#include "NetDialog.h"
#include "ClearSizeDialog.h"
#include <memo/ZipCompressor.h>
#include <QtWidgets>

Form::Form(ProjectSettings* settings, QWidget* parent) : QWidget(parent), projectSettings(settings) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    // Manifest
    auto generalGroupBox = new QGroupBox(tr("Manifest"));

    auto generalGridLayout = new QGridLayout;
    generalGroupBox->setLayout(generalGridLayout);

    generalGridLayout->addWidget(new QLabel(tr("Path:")), 0, 0);
    manifestLabel = new QLabel;
    generalGridLayout->addWidget(manifestLabel, 0, 1);

    generalGridLayout->addWidget(new QLabel(tr("File template:")), 1, 0);
    templateLineEdit = new QLineEdit;
    generalGridLayout->addWidget(templateLineEdit, 1, 1);

    layout->addWidget(generalGroupBox);

    // Update
    auto updateGroupBox = new QGroupBox(tr("Update"));

    auto updateGridLayout = new QGridLayout;
    updateGroupBox->setLayout(updateGridLayout);

    updateGridLayout->addWidget(new QLabel(tr("Version:")), 0, 0);
    versionLineEdit = new QLineEdit;
    updateGridLayout->addWidget(versionLineEdit, 0, 1);

    updateGridLayout->addWidget(new QLabel(tr("Base version:")), 1, 0);
    baseVersionLineEdit = new QLineEdit;
    updateGridLayout->addWidget(baseVersionLineEdit, 1, 1);

    updateGridLayout->addWidget(new QLabel(tr("Date:")), 2, 0);
    dateLineEdit = new QLineEdit;
    updateGridLayout->addWidget(dateLineEdit, 2, 1);

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
    sizeLayout->addWidget(new QLabel("MacOS"), 0, 2);
    sizeLayout->setColumnStretch(3, 1);

    sizeWindowsLabel = new QLabel;
    sizeLayout->addWidget(sizeWindowsLabel, 1, 0);

    sizeLinuxLabel = new QLabel;
    sizeLayout->addWidget(sizeLinuxLabel, 1, 1);

    sizeMacOSLabel = new QLabel;
    sizeLayout->addWidget(sizeMacOSLabel, 1, 2);

    auto clearSizeButton = new QPushButton(tr("Clear..."));
    connect(clearSizeButton, &QPushButton::clicked, this, &Form::clearSize);
    sizeLayout->addWidget(clearSizeButton, 1, 3, Qt::AlignLeft);

    updateGridLayout->addLayout(sizeLayout, 4, 1);

    updateGridLayout->addWidget(new QLabel(tr("Description:")), 5, 0);
    descriptionTextEdit = new QPlainTextEdit;
    updateGridLayout->addWidget(descriptionTextEdit, 5, 1);

    auto updateButtonLayout = new QHBoxLayout;

    auto downloadManifestButton = new QPushButton(tr("Download..."));
    connect(downloadManifestButton, &QPushButton::clicked, this, &Form::downloadManifest);
    updateButtonLayout->addWidget(downloadManifestButton);

    auto uploadManifestButton = new QPushButton(tr("Upload..."));
    connect(uploadManifestButton, &QPushButton::clicked, this, &Form::uploadManifest);
    updateButtonLayout->addWidget(uploadManifestButton);

    updateGridLayout->addLayout(updateButtonLayout, 6, 0, 2, 0, Qt::AlignLeft);

    updateGridLayout->setRowStretch(2, 0);

    layout->addWidget(updateGroupBox);

    // Builder
    auto builderGroupBox = new QGroupBox(tr("Builder"));

    auto builderLayout = new QVBoxLayout;
    builderGroupBox->setLayout(builderLayout);

    auto contentDirLayout = new QHBoxLayout;
    contentDirLayout->addWidget(new QLabel(tr("Content:")));
    contentDirLineEdit = new QLineEdit;

    connect(contentDirLineEdit, &QLineEdit::textEdited, [this] (const QString& text) {
        projectSettings->setContentDir(text);
    });

    contentDirLayout->addWidget(contentDirLineEdit);
    auto contentDirButton = new QPushButton(tr("Browse..."));
    connect(contentDirButton, &QPushButton::clicked, this, &Form::selectContentDirectory);
    contentDirLayout->addWidget(contentDirButton);

    builderLayout->addLayout(contentDirLayout);

    auto builderButtonLayout = new QHBoxLayout;

    auto buildButton = new QPushButton(tr("Build"));
    connect(buildButton, &QPushButton::clicked, this, &Form::build);
    builderButtonLayout->addWidget(buildButton);

    auto uploadContentButton = new QPushButton(tr("Upload..."));
    connect(uploadContentButton, &QPushButton::clicked, this, &Form::uploadContent);
    builderButtonLayout->addWidget(uploadContentButton, 1, Qt::AlignLeft);

    builderLayout->addLayout(builderButtonLayout);

    layout->addWidget(builderGroupBox);

    // Installer
    auto installerGroupBox = new QGroupBox(tr("Installer"));

    auto installerLayout = new QVBoxLayout;
    installerGroupBox->setLayout(installerLayout);

    auto installerPathLayout = new QHBoxLayout;
    installerPathLayout->addWidget(new QLabel(tr("Path:")));
    installerPathLineEdit = new QLineEdit;

    installerPathLayout->addWidget(installerPathLineEdit);
    auto installerPathButton = new QPushButton(tr("Browse..."));
    connect(installerPathButton, &QPushButton::clicked, this, &Form::selectInstallerFile);
    installerPathLayout->addWidget(installerPathButton);

    installerLayout->addLayout(installerPathLayout);

    auto uploadInstallerButton = new QPushButton(tr("Upload..."));
    connect(uploadInstallerButton, &QPushButton::clicked, this, &Form::uploadInstaller);
    installerLayout->addWidget(uploadInstallerButton, 1, Qt::AlignLeft);

    layout->addWidget(installerGroupBox);

    layout->setStretch(1, 0);

    connect(qApp, &QApplication::focusChanged, this, &Form::onFocusChanged);
}

void Form::populateUpdate(const ListModel::Update& update) {
    versionLineEdit->setText(update.version);
    baseVersionLineEdit->setText(update.baseVersion);
    dateLineEdit->setText(update.date);

    int channelIndex = channelComboBox->findText(update.channel);
    channelComboBox->setCurrentIndex(channelIndex);

    sizeWindowsLabel->setText(update.size.contains(Constants::OS::Windows) ? QString::number(update.size[Constants::OS::Windows]) : "");
    sizeLinuxLabel->setText(update.size.contains(Constants::OS::Linux) ? QString::number(update.size[Constants::OS::Linux]) : "");
    sizeMacOSLabel->setText(update.size.contains(Constants::OS::MacOS) ? QString::number(update.size[Constants::OS::MacOS]) : "");

    descriptionTextEdit->setPlainText(update.description);
}

ListModel::Update Form::update() const {
    ListModel::Update update;
    update.version = versionLineEdit->text();
    update.baseVersion = baseVersionLineEdit->text();
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

void Form::setManifestPath(const QString& path) {
    manifestLabel->setText(path);
}

void Form::setFileTemplate(const QString& fileTemplate) {
    templateLineEdit->setText(fileTemplate);
}

QString Form::fileTemplate() const {
    return templateLineEdit->text();
}

void Form::setContentDir(const QString& contentDir) {
    contentDirLineEdit->setText(contentDir);
}

void Form::setInstallerPath(const QString& installerPath) {
    installerPathLineEdit->setText(installerPath);
}

void Form::clear() {
    templateLineEdit->clear();
    contentDirLineEdit->clear();
    installerPathLineEdit->clear();
    clearUpdate();
}

void Form::clearUpdate() {
    versionLineEdit->clear();
    dateLineEdit->clear();
    channelComboBox->setCurrentIndex(0);
    sizeLinuxLabel->clear();
    sizeWindowsLabel->clear();
    sizeMacOSLabel->clear();
    descriptionTextEdit->clear();
}

void Form::onFocusChanged(QWidget* from, QWidget* to) {
    Q_UNUSED(to)

    QWidget* rootTo = root(to);
    QWidget* rootFrom = root(from);

    if (rootFrom == this && rootTo != this) {
        emit lostFocus();
    }
}

void Form::clearSize() {
    ClearSizeDialog clearSizeDialog;
    if (clearSizeDialog.exec() == QDialog::Accepted) {
        for (const QString& os : clearSizeDialog.selectedOS()) {
            setFileSize(os, 0);
        }
    }
}

void Form::selectContentDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        contentDirLineEdit->setText(directory);
        projectSettings->setContentDir(directory);
    }
}

void Form::selectInstallerFile() {
    QString dir;
    QFileInfo fi(installerPathLineEdit->text());
    if (fi.exists()) {
        dir = fi.absolutePath();
    }

    QString file = QFileDialog::getOpenFileName(this, QString(), dir);

    if (!file.isEmpty()) {
        installerPathLineEdit->setText(file);
        projectSettings->setInstallerPath(file);
    }
}

void Form::build() {
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QTemporaryDir tmpdir;
    QString updateDirName = templateLineEdit->text().replace("$version", versionLineEdit->text());
    QString updateDirPath = tmpdir.path() + "/" + updateDirName;
    QString contentDirPath = contentDirLineEdit->text();

    QDirIterator it(contentDirPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString path = it.next();

        if (it.fileInfo().isDir()) continue;

        QString relativePath = path;
        relativePath = relativePath.remove(contentDirPath);

        QString finalPath = updateDirPath + relativePath;

        QFileInfo fi(finalPath);
        QDir dir;
        dir.mkpath(fi.absolutePath());

        QFile::copy(path, finalPath);
    }

    QString zipDir = projectSettings->projectDir() + "/" + Constants::CurrentOS;
    QDir dir;
    dir.mkpath(zipDir);

    zipPath = zipDir + "/" + updateDirName + ".zip";

    Memo::ZipCompressor::compress(zipPath, updateDirPath + "/");

    QFileInfo fi(zipPath);
    setFileSize(Constants::CurrentOS, fi.size());

    QApplication::restoreOverrideCursor();

    QMessageBox::information(this, tr("Build Complete"), tr("Update successfully created"));
}

void Form::downloadManifest() {
    NetDialog netDialog(NetDialog::DialogType::Download, NetDialog::FileType::Manifest, manifestLabel->text(), projectSettings);
    connect(&netDialog, &NetDialog::fileDownloaded, this, &Form::manifestDownloaded);
    netDialog.exec();
}

void Form::uploadManifest() {
    NetDialog netDialog(NetDialog::DialogType::Upload, NetDialog::FileType::Manifest, manifestLabel->text(), projectSettings);
    netDialog.exec();
}

void Form::uploadContent() {
    if (zipPath.isEmpty() || !QFile::exists(zipPath)) {
        QMessageBox::critical(this, tr("Upload Error"), tr("Nothing to upload"));
        return;
    }

    NetDialog netDialog(NetDialog::DialogType::Upload, NetDialog::FileType::Update, zipPath, projectSettings);
    netDialog.exec();
}

void Form::uploadInstaller() {
    QString installerPath = installerPathLineEdit->text();

    if (installerPath.isEmpty() || !QFile::exists(installerPath)) {
        QMessageBox::critical(this, tr("Upload Error"), tr("Nothing to upload"));
        return;
    }

    NetDialog netDialog(NetDialog::DialogType::Upload, NetDialog::FileType::Installer, installerPath, projectSettings);
    netDialog.exec();
}

QWidget*Form::root(QWidget* child) {
    auto widget = child;

    while (widget != nullptr && widget != this) {
         widget = widget->parentWidget();
    }

    return widget;
}

void Form::setFileSize(const QString& os, qint64 size) {
    QString value = QString::number(size);
    if (os == Constants::OS::Windows) {
        sizeWindowsLabel->setText(value);
    } else if (os == Constants::OS::Linux) {
        sizeLinuxLabel->setText(value);
    } else if (os == Constants::OS::MacOS) {
        sizeMacOSLabel->setText(value);
    }
}
