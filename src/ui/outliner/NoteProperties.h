#pragma once
#include <QDialog>
#include <QDateTime>

class NoteProperties : public QDialog {

public:
    struct Data {
        QString title;
        QString createdAt;
        QString updatedAt;
    };

    NoteProperties(const Data& data);
};
