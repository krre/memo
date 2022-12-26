#include "Settings.h"
#include <QSettings>

namespace Settings {

void setValue(const QString& key, const QVariant& value) {
    QSettings settings;
    settings.setValue(key, value);
}

QVariant value(const QString& key, const QVariant& defaultValue) {
    QSettings settings;
    return settings.value(key, defaultValue);
}

namespace General {

constexpr auto Geometry = "geometry";
constexpr auto Splitter = "splitter";
constexpr auto FilePath = "filePath";
constexpr auto MinimizeOnStartup = "minimizeOnStartup";
constexpr auto HideTrayIcon = "hideTrayIcon";
constexpr auto Language = "language";

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

constexpr auto RecentFilesName = "RecentFiles";
constexpr auto Path = "path";

QStringList pathes() {
    QSettings settings;
    int size = settings.beginReadArray(RecentFilesName);
    QStringList result;

    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        result.append(settings.value(Path).toString());
    }

    return result;
}

void setPathes(const QStringList& pathes) {
    if (pathes.isEmpty()) {
        remove(RecentFilesName);
        return;
    }

    QSettings settings;
    settings.beginWriteArray(RecentFilesName);

    for (int i = 0; i < pathes.count(); i++) {
        settings.setArrayIndex(i);
        settings.setValue(Path, pathes.at(i));
    }

    settings.endArray();
}

}

namespace GlobalHotkey {

constexpr auto Enabled = "GlobalHotkey/enabled";
constexpr auto Hotkey = "GlobalHotkey/hotkey";

bool enabled() {
    return value(Enabled, true).toBool();
}

void setEnabled(bool enabled) {
    setValue(Enabled, enabled);
}

QString hotkey() {
    return value(Hotkey, "Ctrl+Alt+M").toString();
}

void setHotkey(const QString& hotkey) {
    setValue(Hotkey, hotkey);
}

}

namespace Editor {

constexpr auto FontFamily = "Editor/fontFamily";
constexpr auto FontSize = "Editor/fontSize";

QString fontFamily(const QString& defaultFamily) {
    return value(FontFamily, defaultFamily).toString();
}

void setFontFamily(const QString& fontFamily) {
    setValue(FontFamily, fontFamily);
}

int fontSize(int defaultSize) {
    return value(FontSize, defaultSize).toInt();
}

void setFontSize(int fontSize) {
    setValue(FontSize, fontSize);
}

}

namespace Server {

constexpr auto Enabled = "Server/enabled";
constexpr auto Key = "Server/key";
constexpr auto Port = "Server/port";

bool enabled() {
    return value(Enabled).toBool();
}

void setEnabled(bool enabled) {
    setValue(Enabled, enabled);
}

QString key() {
    return value(Key).toString();
}

void setKey(const QString& key) {
    setValue(Key, key);
}

int port() {
    return value(Port, 3128).toInt();
}

void setPort(int port) {
    setValue(Port, port);
}

}

namespace Backups {

constexpr auto Directory = "Backups/directory";

QString directory() {
    return value(Directory).toString();
}

void setDirectory(const QString& directory) {
    setValue(Directory, directory);
}

}

}
