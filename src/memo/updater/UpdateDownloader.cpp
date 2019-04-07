#include "UpdateDownloader.h"
#include "core/App.h"
#include "lib/ZipCompressor.h"
#include <QNetworkReply>
#include <QFile>
#include <QDirIterator>
#include <QDebug>

UpdateDownloader::UpdateDownloader(QObject* parent) : QObject(parent) {
    tmpDir.setAutoRemove(false);
}

void UpdateDownloader::download(const QVector<QUrl>& urls) {
    this->urls = urls;
    totalSize = 0;
    downloadFile();
}

void UpdateDownloader::cancel() {
    emit abort();
}

void UpdateDownloader::downloadFile() {
    if (!urls.isEmpty()) {
        QUrl url = urls.last();
        urls.removeLast();

        auto reply = App::networkAccessManager()->get(QNetworkRequest(url));

        connect(reply, &QNetworkReply::finished, [reply, this] {
            if (reply->isOpen()) {
                saveFile(reply->readAll(), reply->url().fileName());
                reply->close();
                downloadFile();
            }

            reply->deleteLater();
        });

        connect(reply, &QNetworkReply::downloadProgress, [this] (qint64 bytesReceived, qint64 bytesTotal) {
            emit downloadProgress(totalSize + bytesReceived);

            if (bytesReceived == bytesTotal) {
                totalSize += bytesTotal;
            }
        });

        connect(this, &UpdateDownloader::abort, reply, &QNetworkReply::abort);

        connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [reply] (QNetworkReply::NetworkError code) {
           qCritical() << "Failed to download update. Error code" << code;
           reply->deleteLater();
        });
    } else {
        // Collect all updates into one directory
        QString finalUpdateDir = tmpDir.path() + "/update";

        QDir dir;
        dir.mkpath(finalUpdateDir);

        for (const auto& dirPath : updateDirs) {
            QDirIterator it(dirPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

            while (it.hasNext()) {
                QString path = it.next();

                if (it.fileInfo().isDir()) continue;

                QString relativePath = path;
                relativePath = relativePath.remove(dirPath);

                QString finalPath = finalUpdateDir + relativePath;

                if (QFile::exists(finalPath)) {
                    QFile::remove(finalPath);
                }

                QFileInfo fi(finalPath);
                QDir dir;
                dir.mkpath(fi.absolutePath());

                QFile::rename(path, finalPath);
            }

            QDir dir(dirPath);
            dir.removeRecursively();
        }

        emit finished(finalUpdateDir);
    }
}

void UpdateDownloader::saveFile(const QByteArray& data, const QString& fileName) {
    QString dirPath = tmpDir.path();
    QString filePath = dirPath + "/" + fileName;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    } else {
        qCritical() << "Error opening temporary file:" + filePath;
        return;
    }

    MemoLib::ZipCompressor::decompress(filePath, dirPath);
    QFile::remove(filePath);

    updateDirs.append(filePath.remove(".zip"));
}
