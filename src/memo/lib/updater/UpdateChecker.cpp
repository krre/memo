#include "UpdateChecker.h"
#include "core/Context.h"
#include "core/Constants.h"
#include <common/Constants.h>
#include <QtNetwork>

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent),
    m_manifestUrl(QUrl(QString(Const::App::DownloadUrl) + "/update/manifest.json")) {
}

void UpdateChecker::check() {
    QNetworkReply* manifestReply = Context::networkAccessManager()->get(QNetworkRequest(m_manifestUrl));

    connect(manifestReply, &QNetworkReply::finished, [manifestReply, this] () {
        QJsonParseError parseError{};
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
    QVector<Update> updates;
    QVersionNumber currentAppVer = QVersionNumber::fromString(Const::App::Version);

    for (const auto value : manifest["updates"].toArray()) {
        QJsonObject updateObj = value.toObject();

        Update update{};
        update.channel = updateObj["channel"].toString();

        QSettings settings;

        if (!isChannelSuit(settings.value("Updates/channel", Const::DefaultSettings::Channel).toString(), update.channel))  continue;

        update.version = updateObj["version"].toString();

        QVersionNumber updateAppVer = QVersionNumber::fromString(update.version);

        if (updateAppVer <= currentAppVer) {
            break;
        }

        QJsonObject sizeObj = updateObj["size"].toObject();

        // Find valid OS update.
        if (sizeObj.contains(Common::Constants::CurrentOS)) {
            int size = sizeObj[Common::Constants::CurrentOS].toInt();
            if (size <= 0) continue;

            QString urlTemplate = manifest["template"].toString();
            update.baseVersion = updateObj["baseVersion"].toString();
            update.url = m_manifestUrl.resolved(QUrl(QString("./%1/%2.zip").arg(Common::Constants::CurrentOS, urlTemplate.replace("$version", update.version))));
            update.description = updateObj["description"].toString();
            update.date = updateObj["date"].toString();
            update.size = size;

            updates.append(update);
        } else {
            continue;
        }
    }

    emit checkResult(updates);
}

bool UpdateChecker::isChannelSuit(const QString& current, const QString& mapOn) {
    using namespace Common::Constants::Channel;

    if (mapOn == Alpha) return false;
    if (mapOn == Release) return true;
    if (mapOn == Beta && current == Beta) return true;

    return false;
}
