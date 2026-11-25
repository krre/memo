#pragma once
#include <QWidget>

class Settings;

class StandardWindow : public QWidget {
public:
    StandardWindow(Settings* settings);

protected:
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

    virtual QString settingsGroup() const = 0;

    virtual void readSettings(Settings* settings);
    virtual void writeSettings(Settings* settings);

    Settings* settings() const;

private:
    Settings* m_settings = nullptr;
    bool m_settingsLoaded = false;
};
