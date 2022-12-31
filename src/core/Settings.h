#pragma once
#include <QObject>
#include "ISettings.h"

class QSettings;

class Settings : public QObject, public ISettings {
public:
    Settings(QObject* parent = nullptr);

    void loadAll();
    void saveAll();

private:
    void loadGeneral();
    void loadBackups();
    void loadEditor();
    void loadGlobalHotKey();
    void loadRecentFilas();
    void loadServer();

    void saveGeneral();
    void saveBackups();
    void saveEditor();
    void saveGlobalHotKey();
    void saveRecentFilas();
    void saveServer();

    QSettings* m_settings = nullptr;
};
