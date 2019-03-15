#include "UpdateChecker.h"
#include "core/App.h"
#include "core/Constants.h"
#include <QtNetwork>

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent) {

}

void UpdateChecker::check() {
    QNetworkReply* redirectorReply = App::networkAccessManager()->get(QNetworkRequest(QUrl(Constants::Updater::ManifestRedirectorUrl)));

    connect(redirectorReply, &QNetworkReply::finished, [redirectorReply] () {
        QJsonObject redirector = QJsonDocument::fromJson(redirectorReply->readAll()).object();
        qDebug() << redirector["manifest"].toString();

        QNetworkReply* manifestReply = App::networkAccessManager()->get(QNetworkRequest(QUrl(redirector["manifest"].toString())));

        connect(manifestReply, &QNetworkReply::finished, [manifestReply] () {
            QJsonObject manifest = QJsonDocument::fromJson(manifestReply->readAll()).object();
            qDebug() << manifest;
            manifestReply->deleteLater();
        });

        connect(manifestReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [manifestReply] (QNetworkReply::NetworkError code) {
           qCritical() << "Failed to get manifest. Error code" << code;
           manifestReply->deleteLater();
        });

        redirectorReply->deleteLater();
    });

    connect(redirectorReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [redirectorReply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get redirector. Error code" << code;
       redirectorReply->deleteLater();
    });

    emit checkResult({});
}
