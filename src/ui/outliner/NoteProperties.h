#pragma once
#include "ui/dialog/StandardDialog.h"
#include "core/Model.h"

class NoteProperties : public StandardDialog {
    Q_OBJECT
public:
    NoteProperties(const Note& note);
};
