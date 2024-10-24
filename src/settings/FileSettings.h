#pragma once
#include "Settings.h"

class FileSettings : public Settings {
public:
    FileSettings();

    void setApplication(const Application& application) override;
    Application application() const override;

    void setMainWindow(const MainWindow& mainWindow) override;
    MainWindow mainWindow() const override;

    void setBirthdays(const Birthdays& birthdays) override;
    Birthdays birthdays() const override;

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
