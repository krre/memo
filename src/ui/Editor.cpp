#include "Editor.h"
#include <QtWidgets>

Editor::Editor(QWidget* parent) : QTextEdit(parent) {
    setEnabled(false);
}

void Editor::setId(Id id) {
    m_id = id;
}

Id Editor::id() const {
    return m_id;
}

void Editor::setMode(Mode mode) {
    if (mode == m_mode) return;

    m_mode = mode;

    if (mode == Mode::Plain) {
        setPlainText(toMarkdown());
    } else if (mode == Mode::Markdown) {
        setMarkdown(toPlainText());
    }
}

Editor::Mode Editor::mode() const {
    return m_mode;
}

void Editor::setNote(const QString& note) {
    if (m_mode == Mode::Plain) {
        setPlainText(note);
    } else if (m_mode == Mode::Markdown) {
        setMarkdown(note);
    }
}

QString Editor::note() const {
    return m_mode == Mode::Plain ? toPlainText() : toMarkdown();
}

void Editor::focusOutEvent(QFocusEvent* event) {
    emit focusLost();
    QTextEdit::focusOutEvent(event);
}

void Editor::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backtab) {
        emit leave();
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_P) {
        setMode(Mode::Plain);
    } else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_M) {
        setMode(Mode::Markdown);
    } else {
        QTextEdit::keyPressEvent(event);
    }
}
