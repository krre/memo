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
    QJsonArray updates = manifest["updates"].toArray();

    Update latestUpdate{};
    Update qtUpdate{};

    populateUpdate(updates.first().toObject(), latestUpdate);

    QVersionNumber currentAppVer = QVersionNumber::fromString(Constants::App::Version);
    QVersionNumber updateAppVer = QVersionNumber::fromString(latestUpdate.version.app);

    // Updates are absent.
    if (currentAppVer == updateAppVer) {
        emit checkResult(latestUpdate, qtUpdate);
        return;
    }

    // Error update version.
    if (currentAppVer > updateAppVer) {
        qCritical() << "Application version greater than update version";
        emit checkResult(latestUpdate, qtUpdate);
        return;
    }

    QVersionNumber currentQtVer = QVersionNumber::fromString(QT_VERSION_STR);
    QVersionNumber updateQtVer = QVersionNumber::fromString(latestUpdate.version.qt);

#if defined Q_OS_LINUX
    QString currentOS = "linux";
#elif defined Q_OS_WIN
    QString currentOS = "windows";
#elif defined Q_OS_MACOS
    QString currentOS = "macos";
#endif

    // Find valid OS and Qt update.
    for (const auto value : updates) {
        if (!latestUpdate.isValid) {
            populateUpdate(value.toObject(), latestUpdate);

            if (latestUpdate.os.isEmpty() || latestUpdate.os.contains(currentOS)) {
                latestUpdate.isValid = true;
            } else {
                continue;
            }
        }

        if (currentQtVer == updateQtVer) {
            break;
        }

        if (currentQtVer > updateQtVer) {
            qCritical() << "Application Qt version greater than update Qt version";
            break;
        }

        populateUpdate(value.toObject(), qtUpdate);

        if (!qtUpdate.os.isEmpty() && !qtUpdate.os.contains(currentOS)) {
            continue;
        }

        if (!qtUpdate.lite) {
            qtUpdate.isValid = true;
            break;
        }
    }

    emit checkResult(latestUpdate, qtUpdate);
}

void UpdateChecker::populateUpdate(const QJsonObject& obj, Update& update) {
    QJsonObject version = obj["version"].toObject();
    update.version.app = version["app"].toString();
    update.version.qt = version["qt"].toString();
    update.description = obj["description"].toString();
    update.date = obj["date"].toString();

    for (const auto value : obj["os"].toArray()) {
        update.os.append(value.toString());
    }

    update.size = obj["size"].toInt();
    update.channel = obj["channel"].toString();
    update.lite = obj["lite"].toBool();
}
