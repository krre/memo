#include "UploadDialog.h"
#include "ProjectSettings.h"
#include <QtWidgets>
#include <QtNetwork>

UploadDialog::UploadDialog(const QString& updatePath, ProjectSettings* settings, QWidget* parent) :
        QDialog(parent), updatePath(updatePath), projectSettings(settings) {
    setWindowTitle(tr("Upload Update"));

    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &UploadDialog::uploadFinished);

    ProjectSettings::FtpData ftpData = settings->ftpData();

    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel(tr("URL:")), 0, 0);
    urlLineEdit = new QLineEdit(ftpData.url);
    gridLayout->addWidget(urlLineEdit, 0, 1);

    gridLayout->addWidget(new QLabel(tr("Login:")), 1, 0);
    loginLineEdit = new QLineEdit(ftpData.login);
    gridLayout->addWidget(loginLineEdit, 1, 1);

    gridLayout->addWidget(new QLabel(tr("Password:")), 2, 0);
    passwordLineEdit = new QLineEdit(ftpData.password);
    passwordLineEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    gridLayout->addWidget(passwordLineEdit, 2, 1);

    layout->addLayout(gridLayout);

    layout->addWidget(new QLabel(tr("Progress:")));

    progressBar = new QProgressBar;
    QFileInfo fi(updatePath);
    progressBar->setMaximum(static_cast<int>(fi.size()));
    layout->addWidget(progressBar);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    adjustSize();
    resize(500, height());
}

void UploadDialog::accept() {

    file = new QFile(updatePath);
    QFileInfo fi(updatePath);

    QUrl url(urlLineEdit->text() + fi.fileName());
    url.setUserName(loginLineEdit->text());
    url.setPassword(passwordLineEdit->text());
    url.setPort(21);

    if (file->open(QIODevice::ReadOnly)) {
        QNetworkReply* reply = manager->put(QNetworkRequest(url), file);
        connect(reply, &QNetworkReply::uploadProgress, this, &UploadDialog::uploadProgress);
    }

    ProjectSettings::FtpData ftpData;
    ftpData.url = urlLineEdit->text();
    ftpData.login = loginLineEdit->text();
    ftpData.password = passwordLineEdit->text();
    projectSettings->setFtpData(ftpData);
}

void UploadDialog::uploadFinished(QNetworkReply* reply) {
    file->close();
    file->deleteLater();
    reply->deleteLater();

    if (reply->error()) {
        QMessageBox::critical(this, tr("Upload Error"), reply->errorString());
        return;
    }

    QDialog::accept();
}

void UploadDialog::uploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    Q_UNUSED(bytesTotal);
    progressBar->setValue(static_cast<int>(bytesSent));
}
