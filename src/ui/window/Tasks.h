#pragma once
#include "StandardWindow.h"

class Database;

class Tasks : public StandardWindow {
    Q_OBJECT
public:
    Tasks(Database* database, Settings* settings);

protected:
    QString settingsGroup() const override;

private:
    Database* m_database = nullptr;
};
