#include "Editor.h"
#include <QtWidgets>

Editor::Editor(QWidget* parent) : QPlainTextEdit(parent) {

}

void Editor::setId(int id) {
    noteId = id;
}

int Editor::id() const {
    return noteId;
}
