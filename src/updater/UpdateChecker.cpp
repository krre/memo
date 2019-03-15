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
        QJsonObject redirector = QJsonDocument::fromJson(redirectorReply->readAll()).object();
        loadManifest(QUrl(redirector["manifest"].toString()));

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
        QJsonObject manifest = QJsonDocument::fromJson(manifestReply->readAll()).object();

        if (manifest.isEmpty()) {
            loadRedirector();
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
