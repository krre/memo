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
    } else {
        QTextEdit::keyPressEvent(event);
    }
}

void Editor::contextMenuEvent(QContextMenuEvent* event) {
    QMenu* menu = createStandardContextMenu();
    menu->addSeparator();

    auto markdownAction = menu->addAction("Markdown");
    markdownAction->setCheckable(true);
    markdownAction->setChecked(m_mode == Mode::Markdown);

    connect(markdownAction, &QAction::toggled, this, [this] (bool checked) {
        setMode(checked ? Mode::Markdown : Mode::Plain);
    });

    menu->exec(event->globalPos());
    delete menu;
}
