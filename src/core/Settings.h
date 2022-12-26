#pragma once
#include <QVariant>

namespace Settings {
    void setValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant());

namespace General {
    QByteArray geometry();
    void setGeometry(const QByteArray& geometry);

    QByteArray splitter();
    void setSplitter(const QByteArray& splitter);

    QString filePath();
    void setFilePath(const QString& filePath);

    bool minimizeOnStartup();
    void setMinimizeOnStartup(bool minimizeOnStartup);

    bool hideTrayIcon();
    void setHideTrayIcon(bool hideTrayIcon);

    QString language();
    void setLanguage(const QString& language);
}

namespace RecentFiles {
    QStringList pathes();
    void setPathes(const QStringList& pathes);
}

namespace GlobalHotkey {
    bool enabled();
    void setEnabled(bool enabled);

    QString hotkey();
    void setHotkey(const QString& hotkey);
}

namespace Editor {
    QString fontFamily(const QString& defaultFamily = QString());
    void setFontFamily(const QString& fontFamily);

    int fontSize(int defaultSize = 0);
    void setFontSize(int fontSize);
}

namespace Server {
    bool enabled();
    void setEnabled(bool enabled);

    QString key();
    void setKey(const QString& key);

    int port();
    void setPort(int port);
}

namespace Backups {
    QString directory();
    void setDirectory(const QString& directory);
}

}
