#pragma once
#include "../StandardWindow.h"

class Database;

class Tasks : public StandardWindow {
    Q_OBJECT
public:
    Tasks(Database* database, Settings* settings);

protected:
    QString settingsGroup() const override;

private slots:
    void add();
    void edit();
    void remove();

    void openTags();

private:
    Database* m_database = nullptr;
};
