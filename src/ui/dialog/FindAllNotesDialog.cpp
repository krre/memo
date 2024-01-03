#include "FindAllNotesDialog.h"
#include "database/Database.h"
#include <QtWidgets>

FindAllNotesDialog::FindAllNotesDialog(Database* database) : m_database(database) {
    setWindowTitle(tr("Find in All Notes"));

    m_lineEdit = new QLineEdit;
    m_listWidget = new QListWidget;

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Text:")), m_lineEdit);

    auto verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_listWidget, 1);

    setContentLayout(verticalLayout, false);
    resizeToWidth(500);

    auto findButton = new QPushButton(tr("Find"));
    findButton->setEnabled(false);
    findButton->setDefault(true);
    connect(findButton, &QPushButton::clicked, this, &FindAllNotesDialog::find);

    buttonBox()->addButton(findButton, QDialogButtonBox::ActionRole);
    buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(m_lineEdit, &QLineEdit::textChanged, this, [=] (const QString& text) {
        findButton->setEnabled(!text.isEmpty());
    });

    connect(m_listWidget, &QListWidget::currentRowChanged, this, [this] (int currentRow) {
        buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(currentRow >= 0);
    });

    connect(m_listWidget, &QListWidget::doubleClicked, this, &FindAllNotesDialog::accept);

    m_lineEdit->setFocus();
}

Id FindAllNotesDialog::noteId() const {
    return m_listWidget->currentItem()->data(Qt::UserRole).toInt();
}

void FindAllNotesDialog::find() {
    m_listWidget->clear();
    QVector<FindNote> findNotes = m_database->find(m_lineEdit->text());

    for (const auto& note : findNotes) {
        auto item = new QListWidgetItem(note.title, m_listWidget);
        item->setData(Qt::UserRole, note.id);
    }
}
