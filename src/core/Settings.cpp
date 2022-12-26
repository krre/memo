#include "Settings.h"
#include <QSettings>

namespace Settings {

namespace Name {
    constexpr auto Geometry = "geometry";
    constexpr auto Splitter = "splitter";
    constexpr auto FilePath = "filePath";
    constexpr auto MinimizeOnStartup = "minimizeOnStartup";
    constexpr auto HideTrayIcon = "hideTrayIcon";
    constexpr auto Language = "language";

    constexpr auto RecentFiles = "RecentFiles";
    constexpr auto Path = "path";

    constexpr auto GlobalHotkeyEnabled = "GlobalHotkey/enabled";
    constexpr auto GlobalHotkeyHotkey = "GlobalHotkey/hotkey";

    constexpr auto EditorFontFamily = "Editor/fontFamily";
    constexpr auto EditorFontSize = "Editor/fontSize";

    constexpr auto ServerEnabled = "Server/enabled";
    constexpr auto ServerKey = "Server/key";
    constexpr auto ServerPort = "Server/port";

    constexpr auto BackupsDirectory = "Backups/directory";
}

using namespace Name;

void setValue(const QString& key, const QVariant& value) {
    QSettings settings;
    settings.setValue(key, value);
}

QVariant value(const QString& key, const QVariant& defaultValue) {
    QSettings settings;
    return settings.value(key, defaultValue);
}

namespace General {

QByteArray geometry() {
    return value(Geometry).toByteArray();
}

void setGeometry(const QByteArray& geometry) {
    setValue(Geometry, geometry);
}

QByteArray splitter() {
    return value(Splitter).toByteArray();
}

void setSplitter(const QByteArray& splitter) {
    setValue(Splitter, splitter);
}

QString filePath() {
    return value(FilePath).toString();
}

void setFilePath(const QString& filePath) {
    setValue(FilePath, filePath);
}

bool minimizeOnStartup() {
    return value(MinimizeOnStartup, false).toBool();
}

void setMinimizeOnStartup(bool minimizeOnStartup) {
    setValue(MinimizeOnStartup, minimizeOnStartup);
}

bool hideTrayIcon() {
    return value(HideTrayIcon).toBool();
}

void setHideTrayIcon(bool hideTrayIcon) {
    setValue(HideTrayIcon, hideTrayIcon);
}

QString language() {
    return value(Language).toString();
}

void setLanguage(const QString& language) {
    setValue(Language, language);
}

}

namespace RecentFiles {

QStringList pathes() {
    QSettings settings;
    int size = settings.beginReadArray(Name::RecentFiles);
    QStringList result;

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        result.append(settings.value(Path).toString());
    }

    return result;
}

void setPathes(const QStringList& pathes) {
    if (pathes.isEmpty()) {
        remove(Name::RecentFiles);
        return;
    }

    QSettings settings;
    settings.beginWriteArray(Name::RecentFiles);

    for (int i = 0; i < pathes.count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(Path, pathes.at(i));
    }

    settings.endArray();
}

}

namespace GlobalHotkey {

bool enabled() {
    return value(GlobalHotkeyEnabled, true).toBool();
}

void setEnabled(bool enabled) {
    setValue(GlobalHotkeyEnabled, enabled);
}

QString hotkey() {
    return value(GlobalHotkeyHotkey, "Ctrl+Alt+M").toString();
}

void setHotkey(const QString& hotkey) {
    setValue(GlobalHotkeyHotkey, hotkey);
}

}

namespace Editor {

QString fontFamily(const QString& defaultFamily) {
    return value(EditorFontFamily, defaultFamily).toString();
}

void setFontFamily(const QString& fontFamily) {
    setValue(EditorFontFamily, fontFamily);
}

int fontSize(int defaultSize) {
    return value(EditorFontSize, defaultSize).toInt();
}

void setFontSize(int fontSize) {
    setValue(EditorFontSize, fontSize);
}

}

namespace Server {

bool enabled() {
    return value(ServerEnabled).toBool();
}

void setEnabled(bool enabled) {
    setValue(ServerEnabled, enabled);
}

QString key() {
    return value(ServerKey).toString();
}

void setKey(const QString& key) {
    setValue(ServerKey, key);
}

int port() {
    return value(ServerPort, 3128).toInt();
}

void setPort(int port) {
    setValue(ServerPort, port);
}

}

namespace Backups {

QString directory() {
    return value(BackupsDirectory).toString();
}

void setDirectory(const QString& directory) {
    setValue(BackupsDirectory, directory);
}

}

}
