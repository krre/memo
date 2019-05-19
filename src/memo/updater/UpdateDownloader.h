#pragma once
#include <QObject>
#include <QVector>
#include <QUrl>
#include <QTemporaryDir>

class UpdateDownloader : public QObject {
    Q_OBJECT
public:
    UpdateDownloader(QObject* parent = nullptr);
    void download(const QUrl& url);
    void cancel();

signals:
    void downloadProgress(qint64 bytes);
    void finished(const QString& updateDir);
    void abort();

private:
    void prepareUpdate();
    void saveFile(const QByteArray& data, const QString& fileName);
    QString loaderName() const;

    QUrl url;
    QString updateDir;
    QTemporaryDir tmpDir;
};
