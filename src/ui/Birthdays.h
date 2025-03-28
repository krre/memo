#pragma once
#include "core/Id.h"
#include <QWidget>

constexpr auto BirthdayDateFormat = "dd.MM.yyyy";

class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QVBoxLayout;
class QCheckBox;

class Database;
class Settings;

class Birthdays : public QWidget {
    Q_OBJECT
public:
    Birthdays(Database* database, Settings* settings);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void add();
    void edit();
    void deleteBirthday();

    void load();
    void updateButtonsState();
    void onCellChanged(int row, int column);

private:
    enum class Column {
        Id,
        Date,
        Name,
        Age,
    };

    void readSettings();
    void writeSettings();

    void addRow(Id id, const QDate& date, const QString& name);
    int age(const QDate& date);

    QTableWidget* createTable();
    QVBoxLayout* createButtons();

    QTableWidget* m_table = nullptr;
    QPushButton* m_editButton = nullptr;
    QPushButton* m_deleteButton = nullptr;

    QCheckBox* m_todayCheckBox = nullptr;

    Database* m_database = nullptr;
    Settings* m_settings = nullptr;
};
