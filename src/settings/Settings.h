#pragma once
#include <QByteArray>
#include <QStringList>

class Settings {
public:
    struct General {
        QString filePath;
        QString language;
        bool minimizeOnStartup;
        bool hideTrayIcon;
    };

    struct MainWindow {
        QByteArray geometry;
        QByteArray state;
        QByteArray splitter;
    };

    struct Backups {
        QString directory;
    };

    struct Editor {
        QString fontFamily;
        int fontSize;
    };

    struct GlobalHotkey {
        bool enabled;
        QString hotkey;
    };

    struct Recent {
        QStringList files;
    };

    struct Server {
        bool enabled;
        QString token;
        int port;
        bool sslEnabled;
        QString certificate;
        QString privateKey;
    };

    virtual void setGeneral(const General& general) = 0;
    virtual General general() const = 0;

    virtual void setMainWindow(const MainWindow& mainWindow) = 0;
    virtual MainWindow mainWindow() const = 0;

    virtual void setBackups(const Backups& backups) = 0;
    virtual Backups backups() const = 0;

    virtual void setEditor(const Editor& editor) = 0;
    virtual Editor editor() const = 0;

    virtual void setGlobalHotkey(const GlobalHotkey& globalHotkey) = 0;
    virtual GlobalHotkey globalHotkey() const = 0;

    virtual void setRecent(const Recent& recent) = 0;
    virtual Recent recent() const = 0;

    virtual void setServer(const Server& server) = 0;
    virtual Server server() const = 0;
};
