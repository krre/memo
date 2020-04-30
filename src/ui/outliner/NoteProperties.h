#pragma once
#include <QDialog>
#include <QDateTime>

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
