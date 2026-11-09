#include "FindText.h"
#include "Editor.h"
#include "core/Application.h"
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextCursor>
#include <QTextDocument>

FindText::FindText(Editor* editor, QWidget* parent)
    : QObject(parent), m_editor(editor), m_dialogParent(parent) {

}

void FindText::find() {
    bool ok;
    QString text = QInputDialog::getText(m_dialogParent, tr("Find"), tr("Text:"), QLineEdit::Normal, m_text, &ok);

    if (!ok || text.isEmpty()) {
        return;
    }

    m_text = text;

    QTextCursor savedCursor = m_editor->textCursor();

    QTextCursor cursor = savedCursor;
    cursor.movePosition(QTextCursor::Start);
    m_editor->setTextCursor(cursor);

    if (!m_editor->find(m_text)) {
        QMessageBox::warning(m_dialogParent, Application::Name, tr("Text not found"));
        m_editor->setTextCursor(savedCursor);
        setNavigationAvailable(false);
        return;
    }

    setNavigationAvailable(true);
}

void FindText::findNext() {
    if (m_text.isEmpty()) return;
    m_editor->find(m_text);
}

void FindText::findPrevious() {
    if (m_text.isEmpty()) return;
    m_editor->find(m_text, QTextDocument::FindBackward);
}

void FindText::reset() {
    m_text.clear();
    setNavigationAvailable(false);
}

void FindText::setNavigationAvailable(bool available) {
    if (m_navigationAvailable == available) return;

    m_navigationAvailable = available;
    emit navigationAvailableChanged(available);
}
