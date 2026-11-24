#pragma once
#include <QWidget>

class Database;
class Settings;

class Tasks : public QWidget {
    Q_OBJECT
public:
    Tasks(Database* database, Settings* settings);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void readSettings();
    void writeSettings();

    Database* m_database = nullptr;
    Settings* m_settings = nullptr;
};
