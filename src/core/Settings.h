#pragma once
#include <QSettings>

class Settings {
public:
    template <typename T>
    class Key {
    public:
        using value_type = T;

        Key(const QString& key, T defaultValue = T{}) : m_key(key), m_default(defaultValue) {}

        const QString& key() const { return m_key; }
        T defaultValue() const { return m_default; }

    private:
        QString m_key;
        T m_default;
    };

    template <typename K>
    static void setValue(K::value_type value) {
        QSettings settings;
        settings.setValue(K().key(), value);
    }

    template <typename K>
    static K::value_type value() {
        K key;
        QSettings settings;
        return settings.value(key.key(), key.defaultValue()).template value<typename K::value_type>();
    }

    template <typename K>
    static bool contains() {
        QSettings settings;
        return settings.contains(K().key());
    }
};

namespace SettingsKey {

namespace General {

struct Geometry : public Settings::Key<QByteArray> {
    Geometry() : Key("geometry") {}
};

struct Splitter : public Settings::Key<QByteArray> {
    Splitter() : Key("splitter") {}
};

struct FilePath : public Settings::Key<QString> {
    FilePath() : Key("filePath") {}
};

struct MinimizeOnStartup : public Settings::Key<bool> {
    MinimizeOnStartup() : Key("minimizeOnStartup") {}
};

struct HideTrayIcon : public Settings::Key<bool> {
    HideTrayIcon() : Key("hideTrayIcon") {}
};

struct Language : public Settings::Key<QString> {
    Language() : Key("language") {}
};

}

namespace Backups {

struct Directory : public Settings::Key<QString> {
    Directory() : Key("Backups/directory") {}
};

}

namespace Editor {

struct FontFamily : public Settings::Key<QString> {
    FontFamily() : Key("Editor/fontFamily") {}
};

struct FontSize : public Settings::Key<int> {
    FontSize() : Key("Editor/fontSize") {}
};

}

namespace GlobalHotkey {

struct Enabled : public Settings::Key<bool> {
    Enabled() : Key("GlobalHotkey/enabled") {}
};

struct Hotkey : public Settings::Key<QString> {
    Hotkey() : Key("GlobalHotkey/hotkey") {}
};

}

namespace RecentFiles {

struct Name : public Settings::Key<QStringList> {
    Name() : Key("RecentFiles/name") {}
};

}

namespace Server {

struct Enabled : public Settings::Key<bool> {
    Enabled() : Key("Server/enabled") {}
};

struct Token : public Settings::Key<QString> {
    Token() : Key("Server/token") {}
};

struct Port : public Settings::Key<int> {
    Port() : Key("Server/port") {}
};

struct Certificate : public Settings::Key<QString> {
    Certificate() : Key("Server/certificate") {}
};

struct PrivateKey : public Settings::Key<QString> {
    PrivateKey() : Key("Server/privateKey") {}
};

}

}
