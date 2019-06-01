#include "Editor.h"
#include <QtWidgets>

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent) {
    setEnabled(false);
}

void Editor::setId(int id) {
    m_id = id;
}

int Editor::id() const {
    return m_id;
}

void Editor::focusOutEvent(QFocusEvent* event) {
    emit focusLost();
    QPlainTextEdit::focusOutEvent(event);
}

void Editor::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backtab) {
        emit leave();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}
