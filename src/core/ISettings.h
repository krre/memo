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
};
