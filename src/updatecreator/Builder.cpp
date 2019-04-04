#include "Builder.h"
#include "ProjectSettings.h"
#include "Constants.h"
#include "lib/Exception.h"
#include <QtWidgets>

Builder::Builder(ProjectSettings* settings, QWidget* parent) : QWidget(parent), projectSettings(settings) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    createAppDirWidgets();
    createFilesWidgets();

    auto buttonLayout = new QHBoxLayout;

    auto refreshButton = new QPushButton(tr("Refresh"));
    connect(refreshButton, &QPushButton::clicked, this, &Builder::refresh);
    buttonLayout->addWidget(refreshButton);

    auto buildButton = new QPushButton(tr("Build"));
    connect(buildButton, &QPushButton::clicked, this, &Builder::build);
    buttonLayout->addWidget(buildButton, 1, Qt::AlignLeft);

    layout->addLayout(buttonLayout);
}

void Builder::load() {
    appDirLineEdit->setText(projectSettings->appDir());

    for (const auto value : projectSettings->snapshot("base")) {
        allFilesListWidget->addItem(value.toObject()["file"].toString());
    }
}

void Builder::clear() {
    appDirLineEdit->clear();
}

void Builder::createSnapshot(const QString& version) {
    QString appDir = appDirLineEdit->text();
    QFileInfo fi(appDir);

    if (!fi.exists() || !fi.isDir()) {
        throw MemoLib::RuntimeError("Wrong application directory");
    }

    QJsonArray snapshot;

    QDirIterator it(appDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString path = it.next();

        if (it.fileInfo().isFile()) {
            QJsonObject item;
            item["hash"] = QString(fileChecksum(path).toHex());

            QString relativePath = path.remove(appDir + "/");
            item["file"] = relativePath;

            allFilesListWidget->addItem(relativePath);
            snapshot.append(item);
        }
    }

    projectSettings->setSnapshot(snapshot, version);
}

void Builder::removeSnapshot(const QString& version) {

}

void Builder::setVersion(const QString& version) {
    this->version = version;
}

void Builder::selectDirectory() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        appDirLineEdit->setText(directory);
        projectSettings->setAppDir(directory);
        createSnapshot(Constants::ZeroVersion);
    }
}

void Builder::refresh() {
    modifiedFiles.clear();

    QJsonArray snapshot = projectSettings->snapshot(version);
    QString appDir = appDirLineEdit->text();

    QDirIterator it(appDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        const QString path = it.next();

        if (it.fileInfo().isFile()) {
            QString relativePath = path;
            relativePath = relativePath.remove(appDir + "/");

            bool isExists = false;

            for (const auto value : snapshot) {
                QString file = value.toObject()["file"].toString();
                if (file == relativePath) {
                    QString hash = QString(fileChecksum(path).toHex());
                    if (hash != value.toObject()["hash"].toString()) {
                        modifiedFiles.append(path);
                        modifiedFilesListWidget->addItem(relativePath);
                    }

                    isExists = true;
                }
            }

            if (!isExists) {
                modifiedFiles.append(path);
                modifiedFilesListWidget->addItem(relativePath);
            }
        }
    }
}

void Builder::build() {
    qDebug() << "build";
}

void Builder::createAppDirWidgets() {
    auto appDirLayout = new QHBoxLayout;
    appDirLayout->addWidget(new QLabel(tr("Application:")));
    appDirLineEdit = new QLineEdit;
    connect(appDirLineEdit, &QLineEdit::textEdited, [this] (const QString& text) {
        projectSettings->setAppDir(text);
    });
    appDirLayout->addWidget(appDirLineEdit);
    auto appDirButton = new QPushButton(tr("Browse..."));
    connect(appDirButton, &QPushButton::clicked, this, &Builder::selectDirectory);
    appDirLayout->addWidget(appDirButton);

    auto mainLayout = static_cast<QVBoxLayout*>(layout());
    mainLayout->addLayout(appDirLayout);
}

void Builder::createFilesWidgets() {
    auto filesGroupBox = new QGroupBox(tr("Files"));

    auto filesLayout = new QVBoxLayout;
    filesGroupBox->setLayout(filesLayout);

    auto splitter = new QSplitter;
    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);

    allFilesListWidget = new QListWidget;
    splitter->addWidget(allFilesListWidget);
    modifiedFilesListWidget = new QListWidget;
    splitter->addWidget(modifiedFilesListWidget);

    filesLayout->addWidget(splitter);

    auto mainLayout = static_cast<QVBoxLayout*>(layout());
    mainLayout->addWidget(filesGroupBox, 1);
}

QByteArray Builder::fileChecksum(const QString& filePath) {
    QFile file(filePath);
    if (file.open(QFile::ReadOnly)) {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (hash.addData(&file)) {
            return hash.result();
        }
    }

    return QByteArray();
}
