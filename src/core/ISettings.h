#pragma once
#include <QVariant>

class ISettings {

public:
    struct General {
        QString filePath;
        QByteArray geometry;
        bool hideTrayIcon;
        QString language;
        bool minimizeOnStartup;
        QByteArray splitter;
    } general;

    struct Backups {
        QString directory;
    } backups;

    struct Editor {
        QString fontFamily;
        int fontSize;
    } editor;

    struct GlobalHotKey {
        bool enabled;
        QString hotKey;
    } globalHotKey;

    struct RecentFiles {
        QStringList path;
    } recentFiles;

    struct Server {
        bool enabled;
        QString key;
        int port;
    } server;

    virtual void loadGeneral() {}
    virtual void loadBackups() {}
    virtual void loadEditor() {}
    virtual void loadGlobalHotKey() {}
    virtual void loadRecentFilas() {}
    virtual void loadServer() {}

    void loadAll() {
        loadGeneral();
        loadBackups();
        loadEditor();
        loadGlobalHotKey();
        loadRecentFilas();
        loadServer();
    }

    virtual void saveGeneral() {}
    virtual void saveBackups() {}
    virtual void saveEditor() {}
    virtual void saveGlobalHotKey() {}
    virtual void saveRecentFilas() {}
    virtual void saveServer() {}

    void saveAll() {
        saveGeneral();
        saveBackups();
        saveEditor();
        saveGlobalHotKey();
        saveRecentFilas();
        saveServer();
    }
};
