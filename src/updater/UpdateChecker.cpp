#include "UpdateChecker.h"
#include "core/App.h"
#include "core/Constants.h"
#include <QtNetwork>

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent) {

}

void UpdateChecker::check() {
    QNetworkReply* reply = App::networkAccessManager()->get(QNetworkRequest(QUrl(Constants::Updater::ManifestRedirectorUrl)));

    connect(reply, &QNetworkReply::finished, [reply] () {
       QJsonObject redirector = QJsonDocument::fromJson(reply->readAll()).object();
       qDebug() << redirector["manifest"].toString();

       reply->deleteLater();
    });

    connect(reply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [reply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get manifest redirector. Error code" << code;
       reply->deleteLater();
    });

    emit checkResult({});
}
