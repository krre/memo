#include "UpdateDownloader.h"
#include <QDebug>

UpdateDownloader::UpdateDownloader(QObject* parent) : QObject(parent) {

}

void UpdateDownloader::download(const QVector<QUrl>& urls) {
    this->urls = urls;

    qDebug() << this->urls;
}
