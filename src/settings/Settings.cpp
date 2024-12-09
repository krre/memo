#include "Settings.h"

void Settings::setApplicationLanguage(const QString& lang) {
    setValue("Application/language", lang);
}

QString Settings::applicationLanguage() const {
    return value("Application/language").toString();
}

void Settings::setApplicationFilePath(const QString& filePath) {
    setValue("Application/filePath", filePath);
}

QString Settings::applicationFilePath() const {
    return value("Application/filePath").toString();
}

void Settings::setApplicationMinimizeOnStartup(bool minimizeOnStartup) {
    setValue("Application/minimizeOnStartup", minimizeOnStartup);
}

bool Settings::applicationMinimizeOnStartup() const {
    return value("Application/minimizeOnStartup", true).toBool();
}

void Settings::setApplicationHideTrayIcon(bool hideTrayIcon) {
    setValue("Application/hideTrayIcon", hideTrayIcon);
}

bool Settings::applicationHideTrayIcon() const {
    return value("Application/hideTrayIcon").toBool();
}

void Settings::setMainWindowGeometry(const QByteArray& geometry) {
    setValue("MainWindow/geometry", geometry);
}

QByteArray Settings::mainWindowGeometry() const {
    return value("MainWindow/geometry").toByteArray();
}

void Settings::setMainWindowState(const QByteArray& state) {
    setValue("MainWindow/state", state);
}

QByteArray Settings::mainWindowState() const {
    return value("MainWindow/state").toByteArray();
}

void Settings::setMainWindowSplitter(const QByteArray& splitter) {
    setValue("MainWindow/splitter", splitter);
}

QByteArray Settings::mainWindowSplitter() const {
    return value("MainWindow/splitter").toByteArray();
}

void Settings::setBirthdaysGeometry(const QByteArray& geometry) {
    setValue("Birthdays/geometry", geometry);
}

QByteArray Settings::birthdaysGeometry() const {
    return value("Birthdays/geometry").toByteArray();
}

void Settings::setBackupsDirectory(const QString& directory) {
    setValue("Backups/directory", directory);
}

QString Settings::backupsDirectory() const {
    return value("Backups/directory").toString();
}

void Settings::setEditorFontFamily(const QString& fontFamily) {
    setValue("Editor/fontFamily", fontFamily);
}

QString Settings::editorFontFamily() const {
    return value("Editor/fontFamily").toString();
}

void Settings::setEditorFontSize(int fontSize) {
    setValue("Editor/fontSize", fontSize);
}

int Settings::editorFontSize() const {
    return value("Editor/fontSize").toInt();
}

void Settings::setGlobalHotkeyEnabled(bool enabled) {
    setValue("GlobalHotkey/enabled", enabled);
}

bool Settings::globalHotkeyEnabled() const {
    return value("GlobalHotkey/enabled", true).toBool();
}

void Settings::setGlobalHotkeyValue(const QString& value) {
    setValue("GlobalHotkey/value", value);
}

QString Settings::globalHotkeyValue() const {
    return value("GlobalHotkey/value", "Ctrl+Alt+M").toString();
}

void Settings::setRecentFiles(const QStringList& recentFiles) {
    QVariantList list;

    for (const auto& file : recentFiles) {
        list.append(file);
    }

    setList("RecentFiles/file", list);
}

QStringList Settings::recentFiles() const {
    QStringList result;

    for (const auto& value : list("RecentFiles/file")) {
        result.append(value.toString());
    }

    return result;
}

void Settings::setServerEnabled(bool enabled) {
    setValue("Server/enabled", enabled);
}

bool Settings::serverEnabled() const {
    return value("Server/enabled").toBool();
}

void Settings::setServerToken(const QString& token) {
    setValue("Server/token", token);
}

QString Settings::serverToken() const {
    return value("Server/token").toString();
}

void Settings::setServerPort(int port) {
    setValue("Server/port", port);
}

int Settings::serverPort() const {
    return value("Server/port").toInt();
}

void Settings::setServerSslEnabled(bool enabled) {
    setValue("Server/sslEnabled", enabled);
}

bool Settings::serverSslEnabled() const {
    return value("Server/sslEnabled").toBool();
}

void Settings::setServerCertificate(const QString& certificate) {
    setValue("Server/certificate", certificate);
}

QString Settings::serverCertificate() const {
    return value("Server/certificate").toString();
}

void Settings::setServerPrivateKey(const QString& privateKey) {
    setValue("Server/privateKey", privateKey);
}

QString Settings::serverPrivateKey() const {
    return value("Server/privateKey").toString();
}
