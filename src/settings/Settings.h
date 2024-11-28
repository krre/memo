#pragma once
#include <QVariant>

class Settings {
public:
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

    void setApplicationLanguage(const QString& lang);
    QString applicationLanguage() const;

    void setApplicationFilePath(const QString& filePath);
    QString applicationFilePath() const;

    void setApplicationMinimizeOnStartup(bool minimizeOnStartup);
    bool applicationMinimizeOnStartup() const;

    void setApplicationHideTrayIcon(bool hideTrayIcon);
    bool applicationHideTrayIcon() const;

    void setMainWindowGeometry(const  QByteArray& geometry);
    QByteArray mainWindowGeometry() const;

    void setMainWindowState(const  QByteArray& state);
    QByteArray mainWindowState() const;

    void setMainWindowSplitter(const  QByteArray& splitter);
    QByteArray mainWindowSplitter() const;

    void setBirthdaysGeometry(const  QByteArray& geometry);
    QByteArray birthdaysGeometry() const;

    void setBackupsDirectory(const QString& directory);
    QString backupsDirectory() const;

    void setEditorFontFamily(const QString& fontFamily);
    QString editorFontFamily() const;

    void setEditorFontSize(int fontSize);
    int editorFontSize() const;

    void setGlobalHotkeyEnabled(bool enabled);
    bool globalHotkeyEnabled() const;

    void setGlobalHotkeyValue(const QString& value);
    QString globalHotkeyValue() const;

    virtual void setRecent(const Recent& recent) = 0;
    virtual Recent recent() const = 0;

    virtual void setServer(const Server& server) = 0;
    virtual Server server() const = 0;

protected:
    virtual void setValue(const QString& key, const QVariant& value) = 0;
    virtual QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const = 0;

    virtual void setList(const QString& key, const QVariantList& list) = 0;
    virtual QVariantList list(const QString& key) const = 0;
};
