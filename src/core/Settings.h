#pragma once
#include <QObject>

class QSettings;

class Settings : public QObject {
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

    Settings(QObject* parent = nullptr);

    void load();
    void save();

private:
    void loadGeneral();
    void loadBackups();
    void loadEditor();
    void loadGlobalHotKey();
    void loadRecentFilas();
    void loadServer();

    void saveGeneral();
    void saveBackups();
    void saveEditor();
    void saveGlobalHotKey();
    void saveRecentFilas();
    void saveServer();

    QSettings* m_settings = nullptr;
};
