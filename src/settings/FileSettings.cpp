#include "FileSettings.h"
#include <QSettings>

FileSettings::FileSettings() {}

void FileSettings::setGeneral(const General& general) {
    QSettings settings;
    settings.beginGroup("General");

    settings.setValue("filePath", general.filePath);
    settings.setValue("language", general.language);
    settings.setValue("minimizeOnStartup", general.minimizeOnStartup);
    settings.setValue("hideTrayIcon", general.hideTrayIcon);

    settings.endGroup();
}

Settings::General FileSettings::general() const {
    General result;

    QSettings settings;
    settings.beginGroup("General");

    result.filePath = settings.value("filePath").toString();
    result.language = settings.value("language").toString();
    result.minimizeOnStartup = settings.value("minimizeOnStartup").toBool();
    result.hideTrayIcon = settings.value("hideTrayIcon").toBool();

    settings.endGroup();

    return result;
}

void FileSettings::setMainWindow(const MainWindow& mainWindow) {
    QSettings settings;
    settings.beginGroup("MainWindow");

    settings.setValue("geometry", mainWindow.geometry);
    settings.setValue("state", mainWindow.state);
    settings.setValue("splitter", mainWindow.splitter);

    settings.endGroup();
}

Settings::MainWindow FileSettings::mainWindow() const {
    MainWindow result;

    QSettings settings;
    settings.beginGroup("MainWindow");

    result.geometry = settings.value("geometry").toByteArray();
    result.state = settings.value("state").toByteArray();
    result.splitter = settings.value("splitter").toByteArray();

    settings.endGroup();

    return result;
}

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
    result.certificate = settings.value("certificate").toString();
    result.privateKey = settings.value("privateKey").toString();

    settings.endGroup();

    return result;
}
