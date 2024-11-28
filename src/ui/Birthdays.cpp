#include "Birthdays.h"
#include "database/Database.h"
#include "core/Application.h"
#include "settings/Settings.h"
#include <QtWidgets>

Birthdays::Birthdays(Database* database, Settings* settings) : m_database(database), m_settings(settings) {
    setWindowTitle(tr("Birthdays"));

    auto horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(createTable());
    horizontalLayout->addLayout(createButtons());

    setLayout(horizontalLayout);

    setAttribute(Qt::WA_DeleteOnClose, true);

    if (database->isBirthdayToday()) {
        m_todayCheckBox->setChecked(true);
    }

    load();

    readSettings();
}

void Birthdays::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void Birthdays::add() {
    Birthday birthday;
    birthday.date = QDate::currentDate();

    Id id = m_database->insertBirthday(birthday);

    const bool isBlocked = m_table->blockSignals(true);
    addRow(id, birthday.date, "");
    m_table->blockSignals(isBlocked);
}

void Birthdays::edit() {
    m_table->editItem(m_table->item(m_table->currentIndex().row(), m_table->currentIndex().column()));
}

void Birthdays::deleteBirthday() {
    if (QMessageBox::question(this, Application::Name, tr("Are you want to delete your birthday?")) == QMessageBox::Yes) {
        int row = m_table->currentRow();
        Id id = m_table->item(row, int(Column::Id))->text().toInt();

        m_database->removeBirthday(id);
        m_table->removeRow(row);

        m_table->setCurrentCell(-1, 0);
    }
}

void Birthdays::updateButtonsState() {
    m_editButton->setEnabled(m_table->currentRow() >= 0);
    m_deleteButton->setEnabled(m_table->currentRow() >= 0);
}

void Birthdays::readSettings() {
    QByteArray geometry = m_settings->birthdaysGeometry();

    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        resize(800, 400);
    }
}

void Birthdays::writeSettings() {
    m_settings->setBirthdaysGeometry(saveGeometry());
}

void Birthdays::onCellChanged(int row, int column [[maybe_unused]]) {
    Birthday birthday;
    birthday.id = m_table->item(row, int(Column::Id))->text().toInt();
    birthday.date = QDate::fromString(m_table->item(row, int(Column::Date))->text(), BirthdayDateFormat);
    birthday.name = m_table->item(row, int(Column::Name))->text();

    m_database->updateBirthday(birthday);
    m_table->item(row, int(Column::Age))->setText(QString::number(age(birthday.date)));
}

void Birthdays::load() {
    m_table->setRowCount(0);
    const bool isBlocked = m_table->blockSignals(true);

    QDate date = m_todayCheckBox->isChecked() ? QDate::currentDate() : QDate();

    for (const auto& birthday : m_database->birthdays(date)) {
        addRow(birthday.id, birthday.date, birthday.name);
    }

    m_table->resizeColumnsToContents();

    m_table->blockSignals(isBlocked);
}

void Birthdays::addRow(Id id, const QDate& date, const QString& name) {
    m_table->insertRow(m_table->rowCount());

    QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(id));
    m_table->setItem(m_table->rowCount() - 1, int(Column::Id), idItem);

    QTableWidgetItem* dateItem = new QTableWidgetItem(date.toString(BirthdayDateFormat));
    m_table->setItem(m_table->rowCount() - 1, int(Column::Date), dateItem);

    QTableWidgetItem* nameItem = new QTableWidgetItem(name);
    m_table->setItem(m_table->rowCount() - 1, int(Column::Name), nameItem);

    QTableWidgetItem* ageItem = new QTableWidgetItem(QString::number(age(date)));
    ageItem->setFlags(ageItem->flags() ^ Qt::ItemIsEditable);
    m_table->setItem(m_table->rowCount() - 1, int(Column::Age), ageItem);
}

int Birthdays::age(const QDate& date) {
    int result = QDate::currentDate().year() - date.year();

    if (QDate::currentDate().dayOfYear() < date.dayOfYear()) {
        result -= 1;
    }

    return result;
}

QTableWidget* Birthdays::createTable() {
    QStringList labels = { "Id", tr("Date"), tr("Name"), tr("Age") };

    m_table = new QTableWidget(0, labels.count());
    m_table->setHorizontalHeaderLabels(labels);
    m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_table->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->setColumnHidden(int(Column::Id), true);

    connect(m_table, &QTableWidget::currentCellChanged, this, &Birthdays::updateButtonsState);
    connect(m_table, &QTableWidget::cellChanged, this, &Birthdays::onCellChanged);

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

    m_todayCheckBox = new QCheckBox(tr("Today"));
    connect(m_todayCheckBox, &QCheckBox::clicked, this, &Birthdays::load);

    auto buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(m_editButton);
    buttonLayout->addWidget(m_deleteButton);
    buttonLayout->addWidget(m_todayCheckBox);
    buttonLayout->addStretch();

    return buttonLayout;
}
