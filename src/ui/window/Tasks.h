#pragma once
#include "StandardWindow.h"

class Database;

class Tasks : public StandardWindow {
    Q_OBJECT
public:
    Tasks(Database* database, Settings* settings);

protected:
    void readSettings(Settings* settings) override;
    void writeSettings(Settings* settings) override;

    Database* m_database = nullptr;
};
