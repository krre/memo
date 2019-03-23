#include "UpdateDownloader.h"
#include "core/App.h"
#include <QNetworkReply>
#include <QFile>
#include <QtGui/private/qzipreader_p.h>
#include <QDebug>

UpdateDownloader::UpdateDownloader(QObject* parent) : QObject(parent) {

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
        emit finished();
    }
}

void UpdateDownloader::saveFile(const QByteArray& data, const QString& fileName) {
    QString dirPath = "/home/krre/1";
    QString filePath = dirPath + "/" + fileName;
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
    } else {
        qCritical() << "Error opening temporary file:" + filePath;
        return;
    }

    QZipReader zipReader(filePath, QIODevice::ReadOnly);
    bool result = zipReader.extractAll(dirPath);
    zipReader.close();

    QFile::remove(filePath);

    if (!result) {
        qCritical() << "Error extracting archive:" << dirPath;
        return;
    }
}
