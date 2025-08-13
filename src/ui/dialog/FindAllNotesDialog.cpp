#include "FindAllNotesDialog.h"
#include "database/Database.h"
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFormLayout>

FindAllNotesDialog::FindAllNotesDialog(Database* database) : m_database(database) {
    setWindowTitle(tr("Find in All Notes"));

    m_lineEdit = new QLineEdit;
    m_listWidget = new QListWidget;
    m_textEdit = new QPlainTextEdit;
    m_textEdit->setReadOnly(true);

    auto formLayout = new QFormLayout;
    formLayout->addRow(new QLabel(tr("Text:")), m_lineEdit);

    auto verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_listWidget);
    verticalLayout->addWidget(m_textEdit);

    setContentLayout(verticalLayout, false);
    resizeToWidth(500);

    auto findButton = new QPushButton(tr("Find"));
    findButton->setEnabled(false);
    findButton->setDefault(true);
    connect(findButton, &QPushButton::clicked, this, &FindAllNotesDialog::find);

    buttonBox()->addButton(findButton, QDialogButtonBox::ActionRole);
    buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(false);
    buttonBox()->button(QDialogButtonBox::Ok)->setText(tr("Go"));

    connect(m_lineEdit, &QLineEdit::textChanged, this, [=] (const QString& text) {
        findButton->setEnabled(!text.isEmpty());
    });

    connect(m_listWidget, &QListWidget::currentRowChanged, this, &FindAllNotesDialog::onCurrentRowChanged);
    connect(m_listWidget, &QListWidget::doubleClicked, this, &FindAllNotesDialog::accept);

    m_lineEdit->setFocus();
}

Id FindAllNotesDialog::noteId() const {
    return m_listWidget->currentItem()->data(Qt::UserRole).toInt();
}

void FindAllNotesDialog::find() {
    m_listWidget->clear();
    const QVector<FindNote> findNotes = m_database->find(m_lineEdit->text());

    for (const auto& note : findNotes) {
        auto item = new QListWidgetItem(note.title, m_listWidget);
        item->setData(Qt::UserRole, note.id.toVariant());
    }

    if (findNotes.count()) {
        m_listWidget->setCurrentRow(0);
    }
}

void FindAllNotesDialog::onCurrentRowChanged(int currentRow) {
    buttonBox()->button(QDialogButtonBox::Ok)->setEnabled(currentRow >= 0);
    m_listWidget->setFocus();
    m_textEdit->clear();
    if (currentRow < 0) return;

    buttonBox()->button(QDialogButtonBox::Ok)->setDefault(true);
    m_textEdit->insertPlainText(m_database->note(noteId()).note);
    highlightWord();
}

void FindAllNotesDialog::highlightWord() {
    m_textEdit->setExtraSelections({});
    if (m_lineEdit->text().isEmpty())  return;

    QTextCharFormat fmt;
    fmt.setBackground(QColor("yellow"));

    QVector<QTextEdit::ExtraSelection> extraSelections;
    QTextCursor cursor = m_textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);

    QTextCursor firstCursor;
    bool firstFound = false;

    while (true) {
        cursor = m_textEdit->document()->find(m_lineEdit->text(), cursor);
        if (cursor.isNull()) break;

        if (!firstFound) {
            firstCursor = cursor;
            firstFound = true;
        }

        QTextEdit::ExtraSelection sel;
        sel.cursor = cursor;
        sel.format = fmt;
        extraSelections.append(sel);
    }

    m_textEdit->setExtraSelections(extraSelections);

    if (firstFound) {
        m_textEdit->setTextCursor(firstCursor);
    }
}
