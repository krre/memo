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

void Editor::focusOutEvent(QFocusEvent* event) {
    emit focusLost();
    QTextEdit::focusOutEvent(event);
}

void Editor::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backtab) {
        emit leave();
    } else {
        QTextEdit::keyPressEvent(event);
    }
}
