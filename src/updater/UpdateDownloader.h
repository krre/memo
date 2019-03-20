#pragma once
#include <QObject>
#include <QVector>
#include <QUrl>

class UpdateDownloader : public QObject {
    Q_OBJECT

public:
    UpdateDownloader(QObject* parent = nullptr);
    void download(const QVector<QUrl>& urls);
    void cancel();

signals:
    void downloadProgress(qint64 bytes);
    void finished();
    void abort();

private:
    void downloadFile();
    void saveFile(const QByteArray& data);

    QVector<QUrl> urls;
    qint64 totalSize = 0;
};
