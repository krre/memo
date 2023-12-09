#pragma once
#include "core/Globals.h"
#include <QWidget>

class QTableWidget;
class QTableWidgetItem;
class QPushButton;
class QVBoxLayout;

class Database;

class Birthdays : public QWidget {
    Q_OBJECT
public:
    Birthdays(Database* database);

private slots:
    void add();
    void edit();
    void deleteBirthday();

    void updateButtonsState();
    void onCellChanged(int row, int column);

private:
    enum class Column {
        Id,
        Date,
        Name,
    };

    void load();
    void addRow(Id id, const QDate& date, const QString& name);

    QTableWidget* createTable();
    QVBoxLayout* createButtons();

    QTableWidget* m_table = nullptr;
    QPushButton* m_editButton = nullptr;
    QPushButton* m_deleteButton = nullptr;

    Database* m_datebase = nullptr;
};
