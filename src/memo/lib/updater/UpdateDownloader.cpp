#include "UpdateDownloader.h"
#include "core/Context.h"
#include <common/ZipCompressor.h>
#include <QNetworkReply>
#include <QFile>
#include <QDirIterator>
#include <QApplication>
#include <QDebug>

UpdateDownloader::UpdateDownloader(QObject* parent) : QObject(parent) {
    m_tmpDir.setAutoRemove(false);
}

void UpdateDownloader::download(const QUrl& url) {
    auto reply = Context::networkAccessManager()->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, [reply, this] {
        if (reply->isOpen()) {
            saveFile(reply->readAll(), reply->url().fileName());
            reply->close();
            prepareUpdate();
        }

        reply->deleteLater();
    });

    connect(reply, &QNetworkReply::downloadProgress, [this] (qint64 bytesReceived, qint64 bytesTotal) {
        Q_UNUSED(bytesTotal)
        emit downloadProgress(bytesReceived);
    });

    connect(this, &UpdateDownloader::abort, reply, &QNetworkReply::abort);

    connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [reply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to download update. Error code:" << code;
       reply->deleteLater();
    });
}

void UpdateDownloader::cancel() {
    emit abort();
}

void UpdateDownloader::prepareUpdate() {
    // Update loader.
    QString loaderSrcPath = m_updateDir + "/" + loaderName();
    // Fix bug with unseting x-flag after decompressing zip on non-Windows OS.
    QFile::setPermissions(loaderSrcPath, QFile::permissions(loaderSrcPath) | QFile::ExeOwner);

    if (QFile::exists(loaderSrcPath)) {
        QString loaderDstPath = qApp->applicationDirPath() + "/" + loaderName();
        QFile::remove(loaderDstPath);
        QFile::rename(loaderSrcPath, loaderDstPath);
    }

    emit finished(m_updateDir);
}

void UpdateDownloader::saveFile(const QByteArray& data, const QString& fileName) {
    QString dirPath = m_tmpDir.path();
    QString filePath = dirPath + "/" + fileName;
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    } else {
        qCritical() << "Error opening temporary file:" << filePath;
        return;
    }

    Common::ZipCompressor::decompress(filePath, dirPath);
    QFile::remove(filePath);

    m_updateDir = filePath.remove(".zip");
}

QString UpdateDownloader::loaderName() const {
#ifdef Q_OS_WIN
    return "loader.exe";
#else
    return "loader";
#endif
}
