#pragma once
#include "Settings.h"

class FileSettings : public Settings {
public:
    FileSettings();

    void setGeneral(const General& general) override;
    General general() const override;

    void setMainWindow(const MainWindow& mainWindow) override;
    MainWindow mainWindow() const override;
    bool containsGeometry() const override;

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
};
