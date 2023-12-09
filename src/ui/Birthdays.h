#pragma once
#include <QWidget>

class QTableWidget;
class QPushButton;
class QVBoxLayout;

class Birthdays : public QWidget {
    Q_OBJECT
public:
    Birthdays();

private slots:
    void add();
    void edit();
    void deleteBirthday();

    void updateButtonsState();

private:
    enum class Column {
        Date,
        Name,
    };

    void addRow(const QDate& date, const QString& name);

    QTableWidget* createTable();
    QVBoxLayout* createButtons();

    QTableWidget* m_table = nullptr;
    QPushButton* m_editButton = nullptr;
    QPushButton* m_deleteButton = nullptr;
};
