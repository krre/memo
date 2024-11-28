#pragma once
#include "Settings.h"

class FileSettings : public Settings {
public:
    FileSettings();

    void setBackups(const Backups& backups) override;
    Backups backups() const override;

    void setEditor(const Editor& editor) override;
    Editor editor() const override;

    void setGlobalHotkey(const GlobalHotkey& globalHotkey) override;
    GlobalHotkey globalHotkey() const override;

    void setRecent(const Recent& recent) override;
    Recent recent() const override;

    void setServer(const Server& server) override;
    Server server() const override;

protected:
    void setValue(const QString& key, const QVariant& value) override;
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const override;
};
