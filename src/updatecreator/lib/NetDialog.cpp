#include "NetDialog.h"
#include "ProjectSettings.h"
#include <common/Constants.h>
#include <QtWidgets>
#include <QtNetwork>

NetDialog::NetDialog(DialogType dialogType, FileType fileType, const QString& filePath, ProjectSettings* projectSettings, QWidget* parent) :
        QDialog(parent), dialogType(dialogType), fileType(fileType), filePath(filePath), projectSettings(projectSettings) {
    setWindowTitle(dialogType == DialogType::Upload ? tr("Upload File") : tr("Download File"));

    manager = new QNetworkAccessManager(this);

    ProjectSettings::FtpData ftpData = projectSettings->ftpData();

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto gridLayout = new QGridLayout;

    QFileInfo fi(filePath);

    gridLayout->addWidget(new QLabel(tr("File:")), 0, 0);
    auto fileLineEdit = new QLineEdit(fi.fileName());
    fileLineEdit->setReadOnly(true);
    gridLayout->addWidget(fileLineEdit, 0, 1);

    gridLayout->addWidget(new QLabel(tr("URL:")), 1, 0);
    urlLineEdit = new QLineEdit(fileType == FileType::Installer ? ftpData.installUrl : ftpData.updateUrl);
    gridLayout->addWidget(urlLineEdit, 1, 1);

    gridLayout->addWidget(new QLabel(tr("Login:")), 2, 0);
    loginLineEdit = new QLineEdit(ftpData.login);
    gridLayout->addWidget(loginLineEdit, 2, 1);

    gridLayout->addWidget(new QLabel(tr("Password:")), 3, 0);
    passwordLineEdit = new QLineEdit(ftpData.password);
    passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    gridLayout->addWidget(passwordLineEdit, 3, 1);

    layout->addLayout(gridLayout);

    layout->addWidget(new QLabel(tr("Progress:")));

    progressBar = new QProgressBar;
    progressBar->setMaximum(static_cast<int>(fi.size()));
    layout->addWidget(progressBar);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    adjustSize();
    resize(500, height());
}

void NetDialog::accept() {
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    QFileInfo fi(filePath);

    QUrl url(urlLineEdit->text() + "/" + (fileType == FileType::Manifest ? "" : QString(Common::Constants::CurrentOS) + "/") + fi.fileName());
    url.setUserName(loginLineEdit->text());
    url.setPassword(passwordLineEdit->text());
    url.setPort(21);

    if (dialogType == DialogType::Upload) {
        file = new QFile(filePath);

        if (file->open(QIODevice::ReadOnly)) {
            QNetworkReply* reply = manager->put(QNetworkRequest(url), file);
            connect(reply, &QNetworkReply::uploadProgress, this, &NetDialog::uploadProgress);
            connect(reply, &QNetworkReply::finished, this, [this, reply] () {
                uploadFinished(reply);
            });
        } else {
            QMessageBox::critical(this, tr("Open File Error"), "Error opening file:" + filePath);
            return;
        }
    } else {
        QNetworkReply* reply = manager->get(QNetworkRequest(url));
        connect(reply, &QNetworkReply::downloadProgress, this, &NetDialog::downloadProgress);
        connect(reply, &QNetworkReply::finished, this, [this, reply] () {
            downloadFinished(reply);
        });
    }

    ProjectSettings::FtpData ftpData = projectSettings->ftpData();

    if (fileType == FileType::Installer) {
        ftpData.installUrl = urlLineEdit->text();
    } else {
        ftpData.updateUrl = urlLineEdit->text();
    }

    ftpData.login = loginLineEdit->text();
    ftpData.password = passwordLineEdit->text();
    projectSettings->setFtpData(ftpData);
}

void NetDialog::uploadFinished(QNetworkReply* reply) {
    file->close();
    file->deleteLater();
    reply->deleteLater();

    if (reply->error()) {
        QMessageBox::critical(this, tr("Upload Error"), reply->errorString());
        return;
    }

    QDialog::accept();
}

void NetDialog::downloadFinished(QNetworkReply* reply) {
    reply->deleteLater();

    if (reply->error()) {
        QMessageBox::critical(this, tr("Upload Error"), reply->errorString());
        return;
    }

    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(reply->readAll());
        file.close();
        emit fileDownloaded();
    } else {
        QMessageBox::critical(this, tr("Open File Error"), "Error opening file:" + filePath);
        return;
    }

    QDialog::accept();
}

void NetDialog::uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    Q_UNUSED(bytesTotal);
    progressBar->setValue(static_cast<int>(bytesSent));
}

void NetDialog::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    Q_UNUSED(bytesTotal);
    progressBar->setValue(static_cast<int>(bytesReceived));
}
