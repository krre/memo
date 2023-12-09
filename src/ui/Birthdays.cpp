#include "Birthdays.h"
#include <QtWidgets>

Birthdays::Birthdays() {
    setWindowTitle(tr("Birthdays"));

    auto horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(createTable());
    horizontalLayout->addLayout(createButtons());

    setLayout(horizontalLayout);

    setAttribute(Qt::WA_DeleteOnClose, true);
    resize(800, 400);
}

void Birthdays::add() {
    addRow(QDate::currentDate(), "");
}

void Birthdays::edit() {
    m_table->editItem(m_table->item(m_table->currentIndex().row(), m_table->currentIndex().column()));
}

void Birthdays::deleteBirthday() {
    if (QMessageBox::question(this, tr("Delete Birthday"), tr("Are you want to delete your birthday?")) == QMessageBox::Yes) {
        m_table->removeRow(m_table->currentRow());
        m_table->setCurrentCell(-1, 0);
    }
}

void Birthdays::updateButtonsState() {
    m_editButton->setEnabled(m_table->currentRow() >= 0);
    m_deleteButton->setEnabled(m_table->currentRow() >= 0);
}

void Birthdays::addRow(const QDate& date, const QString& name) {
    m_table->insertRow(m_table->rowCount());

    QTableWidgetItem* dateItem = new QTableWidgetItem(date.toString("dd.MM.yyyy"));
    m_table->setItem(m_table->rowCount() - 1, int(Column::Date), dateItem);

    QTableWidgetItem* nameItem = new QTableWidgetItem(name);
    m_table->setItem(m_table->rowCount() - 1, int(Column::Name), nameItem);
}

QTableWidget* Birthdays::createTable() {
    m_table = new QTableWidget(0, 2);
    m_table->setHorizontalHeaderLabels(QStringList(tr("Date")) << tr("Name"));
    m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed|QAbstractItemView::AnyKeyPressed);
    m_table->horizontalHeader()->setStretchLastSection(true);

    connect(m_table, &QTableWidget::currentCellChanged, this, &Birthdays::updateButtonsState);

    return m_table;
}

QVBoxLayout* Birthdays::createButtons() {
    auto addButton = new QPushButton(tr("Add"));
    connect(addButton, &QPushButton::clicked, this, &Birthdays::add);

    m_editButton = new QPushButton(tr("Edit"));
    m_editButton->setEnabled(false);
    connect(m_editButton, &QPushButton::clicked, this, &Birthdays::edit);

    m_deleteButton = new QPushButton(tr("Delete..."));
    m_deleteButton->setEnabled(false);
    connect(m_deleteButton, &QPushButton::clicked, this, &Birthdays::deleteBirthday);

    auto buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addStretch();

    return buttonLayout;
}
