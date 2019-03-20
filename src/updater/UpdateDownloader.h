#pragma once
#include <QObject>
#include <QVector>
#include <QUrl>

class UpdateDownloader : public QObject {
    Q_OBJECT

public:
    UpdateDownloader(QObject* parent = nullptr);
    void download(const QVector<QUrl>& urls);

private:
    QVector<QUrl> urls;
};
