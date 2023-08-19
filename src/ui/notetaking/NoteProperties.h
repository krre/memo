#pragma once
#include "ui/StandardDialog.h"

class NoteProperties : public StandardDialog {
    Q_OBJECT
public:
    struct Data {
        QString title;
        QString createdAt;
        QString updatedAt;
    };

    NoteProperties(const Data& data);
};
