#pragma once
#include <QObject>
#include "ISettings.h"

class QSettings;

class Settings : public QObject, public ISettings {
public:
    Settings(QObject* parent = nullptr);

    void loadGeneral() override;
    void loadBackups() override;
    void loadEditor() override;
    void loadGlobalHotKey() override;
    void loadRecentFilas() override;
    void loadServer() override;

    void saveGeneral() override;
    void saveBackups() override;
    void saveEditor() override;
    void saveGlobalHotKey() override;
    void saveRecentFilas() override;
    void saveServer() override;

private:
    QSettings* m_settings = nullptr;
};
