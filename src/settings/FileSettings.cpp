#include "FileSettings.h"
#include <QSettings>

FileSettings::FileSettings() {}

void FileSettings::setServer(const Server& server) {
    QSettings settings;
    settings.beginGroup("Server");

    settings.setValue("enabled", server.enabled);
    settings.setValue("token", server.token);
    settings.setValue("port", server.port);
    settings.setValue("sslEnabled", server.sslEnabled);
    settings.setValue("certificate", server.certificate);
    settings.setValue("privateKey", server.privateKey);

    settings.endGroup();
}

Settings::Server FileSettings::server() const {
    Server result;

    QSettings settings;
    settings.beginGroup("Server");

    result.enabled = settings.value("enabled").toBool();
    result.token = settings.value("token").toString();
    result.port = settings.value("port").toInt();
    result.sslEnabled = settings.value("sslEnabled").toBool();
    result.certificate = settings.value("certificate").toString();
    result.privateKey = settings.value("privateKey").toString();

    settings.endGroup();

    return result;
}

void FileSettings::setValue(const QString& key, const QVariant& value) {
    QSettings settings;
    settings.setValue(key, value);
}

QVariant FileSettings::value(const QString& key, const QVariant& defaultValue) const {
    QSettings settings;
    return settings.value(key, defaultValue);
}

void FileSettings::setList(const QString& key, const QVariantList& list) {
    QSettings settings;
    settings.beginWriteArray(key.split("/").first());

    for (int i = 0; i < list.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(key.split("/").last(), list.at(i));
    }

    settings.endArray();
}

QVariantList FileSettings::list(const QString& key) const {
    QVariantList result;

    QSettings settings;
    int size = settings.beginReadArray(key.split("/").first());

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(key.split("/").last()));
    }

    settings.endArray();

    return result;
}
