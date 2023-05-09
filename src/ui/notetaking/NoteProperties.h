#pragma once
#include <QDialog>

class NoteProperties : public QDialog {
    Q_OBJECT
public:
    struct Data {
        QString title;
        QString createdAt;
        QString updatedAt;
    };

    NoteProperties(const Data& data);
};
