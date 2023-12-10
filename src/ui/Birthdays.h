#pragma once
#include "core/Globals.h"
#include <QWidget>

constexpr auto BirthdayDateFormat = "dd.MM.yyyy";

class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QVBoxLayout;
class QCheckBox;

class Database;

class Birthdays : public QWidget {
    Q_OBJECT
public:
    enum class Filter {
        All,
        Today
    };

    Birthdays(Database* database, Filter filter = Filter::All);

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
    };

    void addRow(Id id, const QDate& date, const QString& name);

    QTableWidget* createTable();
    QVBoxLayout* createButtons();

    QTableWidget* m_table = nullptr;
    QPushButton* m_editButton = nullptr;
    QPushButton* m_deleteButton = nullptr;

    QCheckBox* m_todayCheckBox = nullptr;
    Database* m_datebase = nullptr;
};
