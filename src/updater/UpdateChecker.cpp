#include "UpdateChecker.h"
#include "core/App.h"
#include "core/Constants.h"
#include <QtNetwork>

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent) {

}

void UpdateChecker::check() {
    QSettings settings;
    QString manifestUrl = settings.value("Network/manifestUrl").toString();

    if (manifestUrl.isEmpty()) {
        loadRedirector();
    } else {
        loadManifest(QUrl(manifestUrl));
    }
}

void UpdateChecker::loadRedirector() {
    QNetworkReply* redirectorReply = App::networkAccessManager()->get(QNetworkRequest(QUrl(Constants::Updater::ManifestRedirectorUrl)));

    connect(redirectorReply, &QNetworkReply::finished, [redirectorReply, this] () {
        QJsonParseError parseError;
        QJsonObject redirector = QJsonDocument::fromJson(redirectorReply->readAll(), &parseError).object();
        qDebug() << redirector;

        if (parseError.error != QJsonParseError::NoError) {
            qCritical() << QString("Failed to parse redirector at offset %1: %2").arg(parseError.offset).arg(parseError.errorString());
        } else {
            loadManifest(QUrl(redirector["manifest"].toString()));
        }

        redirectorReply->deleteLater();
    });

    connect(redirectorReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [redirectorReply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get redirector. Error code" << code;
       redirectorReply->deleteLater();
    });
}

void UpdateChecker::loadManifest(const QUrl& manifestUrl) {
    QNetworkReply* manifestReply = App::networkAccessManager()->get(QNetworkRequest(manifestUrl));

    connect(manifestReply, &QNetworkReply::finished, [manifestReply, manifestUrl, this] () {
        QJsonParseError parseError;
        QJsonObject manifest = QJsonDocument::fromJson(manifestReply->readAll(), &parseError).object();

        if (parseError.error != QJsonParseError::NoError) {
            qCritical() << QString("Failed to parse manifest at offset %1: %2").arg(parseError.offset).arg(parseError.errorString());
        } else {
            QSettings settings;
            settings.setValue("Network/manifestUrl", manifestUrl.toString());

            findUpdate(manifest);
        }

        manifestReply->deleteLater();
    });

    connect(manifestReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [manifestReply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get manifest. Error code" << code;
       manifestReply->deleteLater();
    });
}

void UpdateChecker::findUpdate(const QJsonObject& manifest) {
    qDebug() << manifest;

    Update update;
    update.isValid = false;

    emit checkResult(update);
}
