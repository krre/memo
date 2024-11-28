#include "FileSettings.h"
#include <QSettings>

FileSettings::FileSettings() {}

void FileSettings::setBackups(const Backups& backups) {
    QSettings settings;
    settings.setValue("Backups/directory", backups.directory);
}

Settings::Backups FileSettings::backups() const {
    Backups result;

    QSettings settings;
    result.directory = settings.value("Backups/directory").toString();

    return result;
}

void FileSettings::setEditor(const Editor& editor) {
    QSettings settings;
    settings.beginGroup("Editor");

    settings.setValue("fontFamily", editor.fontFamily);
    settings.setValue("fontSize", editor.fontSize);

    settings.endGroup();
}

Settings::Editor FileSettings::editor() const {
    Editor result;

    QSettings settings;
    settings.beginGroup("Editor");

    result.fontFamily = settings.value("fontFamily").toString();
    result.fontSize = settings.value("fontSize").toInt();

    settings.endGroup();

    return result;
}

void FileSettings::setGlobalHotkey(const GlobalHotkey& globalHotkey) {
    QSettings settings;
    settings.beginGroup("GlobalHotkey");

    settings.setValue("enabled", globalHotkey.enabled);
    settings.setValue("hotkey", globalHotkey.hotkey);

    settings.endGroup();
}

Settings::GlobalHotkey FileSettings::globalHotkey() const {
    GlobalHotkey result;

    QSettings settings;
    settings.beginGroup("GlobalHotkey");

    result.enabled = settings.value("enabled").toBool();
    result.hotkey = settings.value("hotkey").toString();

    settings.endGroup();

    return result;
}

void FileSettings::setRecent(const Recent& recent) {
    QSettings settings;
    settings.beginWriteArray("Recent");

    for (int i = 0; i < recent.files.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("file", recent.files.at(i));
    }

    settings.endArray();
}

Settings::Recent FileSettings::recent() const {
    Recent result;

    QSettings settings;
    int size = settings.beginReadArray("Recent");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        result.files.append(settings.value("file").toString());
    }

    settings.endArray();
    return result;
}

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
