#pragma once
#include <QObject>
#include <QVector>
#include <QUrl>
#include <QTemporaryDir>

class UpdateDownloader : public QObject {
    Q_OBJECT

public:
    UpdateDownloader(QObject* parent = nullptr);
    void download(const QVector<QUrl>& urls);
    void cancel();

signals:
    void downloadProgress(qint64 bytes);
    void finished(const QString& updateDir);
    void abort();

private:
    void downloadFile();
    void saveFile(const QByteArray& data, const QString& fileName);

    QVector<QUrl> urls;
    QVector<QString> updateDirs;
    qint64 totalSize = 0;
    QTemporaryDir tmpDir;
};
