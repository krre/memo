#include "Settings.h"
#include <QSettings>
#include <QWidget>

namespace General {
    constexpr auto Geometry = "geometry";
    constexpr auto Splitter = "splitter";
    constexpr auto FilePath = "filePath";
    constexpr auto MinimizeOnStartup = "minimizeOnStartup";
    constexpr auto HideTrayIcon = "hideTrayIcon";
    constexpr auto Language = "language";
}

namespace Backups {
    constexpr auto Directory = "Backups/directory";
}

namespace Editor {
    constexpr auto FontFamily = "Editor/fontFamily";
    constexpr auto FontSize = "Editor/fontSize";
}

namespace GlobalHotkey {
    constexpr auto Enabled = "GlobalHotkey/enabled";
    constexpr auto Hotkey = "GlobalHotkey/hotkey";
}

namespace RecentFiles {
    constexpr auto RecentFilesName = "RecentFiles";
    constexpr auto Path = "path";
}

namespace Server {
    constexpr auto Enabled = "Server/enabled";
    constexpr auto Key = "Server/key";
    constexpr auto Port = "Server/port";
}

Settings::Settings(QObject* parent) : QObject(parent) {
    m_settings = new QSettings(this);
}

void Settings::load() {
    loadGeneral();
    loadBackups();
    loadEditor();
    loadGlobalHotKey();
    loadRecentFilas();
    loadServer();
}

void Settings::save() {
    saveGeneral();
    saveBackups();
    saveEditor();
    saveGlobalHotKey();
    saveRecentFilas();
    saveServer();
}

void Settings::loadGeneral() {
    using namespace ::General;

    general.geometry = m_settings->value(Geometry).toByteArray();
    general.filePath = m_settings->value(FilePath).toString();
    general.hideTrayIcon = m_settings->value(HideTrayIcon).toBool();
    general.language = m_settings->value(Language).toString();
    general.minimizeOnStartup = m_settings->value(MinimizeOnStartup, false).toBool();
    general.splitter = m_settings->value(Splitter).toByteArray();
}

void Settings::loadBackups() {
    backups.directory = m_settings->value(::Backups::Directory).toString();
}

void Settings::loadEditor() {
    using namespace ::Editor;

    QWidget widget;
    editor.fontFamily = m_settings->value(FontFamily, widget.font().family()).toString();
    editor.fontSize = m_settings->value(FontSize, widget.font().pointSize()).toInt();
}

void Settings::loadGlobalHotKey() {
    using namespace ::GlobalHotkey;

    globalHotKey.hotKey = m_settings->value(Hotkey, "Ctrl+Alt+M").toString();
    globalHotKey.enabled = m_settings->value(Enabled, true).toBool();
}

void Settings::loadRecentFilas() {
    using namespace ::RecentFiles;

    int size = m_settings->beginReadArray(RecentFilesName);

    for (int i = 0; i < size; i++) {
        m_settings->setArrayIndex(i);
        recentFiles.path.append(m_settings->value(Path).toString());
    }

    m_settings->endArray();
}

void Settings::loadServer() {
    using namespace ::Server;

    server.enabled = m_settings->value(Enabled).toBool();
    server.token = m_settings->value(Key).toString();
    server.port = m_settings->value(Port).toInt();
}

void Settings::saveGeneral() {
    using namespace ::General;

    m_settings->setValue(Geometry, general.geometry);
    m_settings->setValue(FilePath, general.filePath);
    m_settings->setValue(HideTrayIcon, general.hideTrayIcon);
    m_settings->setValue(Language, general.language);
    m_settings->setValue(MinimizeOnStartup, general.minimizeOnStartup);
    m_settings->setValue(Splitter, general.splitter);
}

void Settings::saveBackups() {
    m_settings->setValue(::Backups::Directory, backups.directory);
}

void Settings::saveEditor() {
    using namespace ::Editor;

    m_settings->setValue(FontFamily, editor.fontFamily);
    m_settings->setValue(FontSize, editor.fontSize);
}

void Settings::saveGlobalHotKey() {
    using namespace ::GlobalHotkey;

    m_settings->setValue(Hotkey, globalHotKey.hotKey);
    m_settings->setValue(Enabled, globalHotKey.enabled);
}

void Settings::saveRecentFilas() {
    using namespace ::RecentFiles;

    if (recentFiles.path.isEmpty()) {
        m_settings->remove(RecentFilesName);
        return;
    }

    m_settings->beginWriteArray(RecentFilesName);

    for (int i = 0; i < recentFiles.path.count(); i++) {
        m_settings->setArrayIndex(i);
        m_settings->setValue(Path, recentFiles.path.at(i));
    }

    m_settings->endArray();
}

void Settings::saveServer() {
    using namespace ::Server;

    m_settings->setValue(Enabled, server.enabled);
    m_settings->setValue(Key, server.token);
    m_settings->setValue(Port, server.port);
}
