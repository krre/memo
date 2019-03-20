#include "UpdateChecker.h"
#include "core/App.h"
#include "core/Constants.h"
#include <QtNetwork>

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent), manifestUrl(QUrl(Constants::Updater::ManifestUrl)) {

}

void UpdateChecker::check() {
    QNetworkReply* manifestReply = App::networkAccessManager()->get(QNetworkRequest(manifestUrl));

    connect(manifestReply, &QNetworkReply::finished, [manifestReply, this] () {
        QJsonParseError parseError;
        QJsonObject manifest = QJsonDocument::fromJson(manifestReply->readAll(), &parseError).object();

        if (parseError.error != QJsonParseError::NoError) {
            qCritical() << QString("Failed to parse manifest at offset %1: %2").arg(parseError.offset).arg(parseError.errorString());
        } else {
            findUpdates(manifest);
        }

        manifestReply->deleteLater();
    });

    connect(manifestReply, qOverload<QNetworkReply::NetworkError>(&QNetworkReply::error), [manifestReply] (QNetworkReply::NetworkError code) {
       qCritical() << "Failed to get manifest. Error code" << code;
       manifestReply->deleteLater();
    });
}

void UpdateChecker::findUpdates(const QJsonObject& manifest) {
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
