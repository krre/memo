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

            findUpdates(manifest, manifestUrl);
        }

        manifestReply->deleteLater();
    });

    connect(manifestReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [manifestReply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get manifest. Error code" << code;
       manifestReply->deleteLater();
    });
}

void UpdateChecker::findUpdates(const QJsonObject& manifest, const QUrl& manifestUrl) {
#if defined Q_OS_LINUX
    QString currentOS = "linux";
#elif defined Q_OS_WIN
    QString currentOS = "windows";
#elif defined Q_OS_MACOS
    QString currentOS = "macos";
#endif

    QVector<Update> updates;
    QVersionNumber currentAppVer = QVersionNumber::fromString(Constants::App::Version);

    for (const auto value : manifest["updates"].toArray()) {
        QJsonObject updateObj = value.toObject();

        Update update{};
        update.version = updateObj["version"].toString();

        QVersionNumber updateAppVer = QVersionNumber::fromString(update.version);

        if (updateAppVer <= currentAppVer) {
            break;
        }

        for (const auto value : updateObj["os"].toArray()) {
            update.os.append(value.toString());
        }

        // Find valid OS update.
        if (update.os.isEmpty() || update.os.contains(currentOS)) {
            QString urlTemplate = manifest["url"].toString();
            update.url = manifestUrl.resolved(QUrl(QString("./%1").arg(urlTemplate.replace("$os", currentOS).replace("$version", update.version))));
            update.description = updateObj["description"].toString();
            update.date = updateObj["date"].toString();
            update.size = updateObj["size"].toInt();
            update.channel = updateObj["channel"].toString();

            updates.append(update);
        } else {
            continue;
        }
    }

    emit checkResult(updates);
}
