#include "Editor.h"
#include <QtWidgets>

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent) {
    setEnabled(false);
}

void Editor::setId(int id) {
    noteId = id;
}

int Editor::id() const {
    return noteId;
}

void Editor::focusOutEvent(QFocusEvent* event) {
    Q_UNUSED(event)
    emit focusLost();
}

void Editor::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Backtab) {
        emit leave();
    } else {
        QPlainTextEdit::keyPressEvent(event);
    }
}
