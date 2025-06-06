#pragma once
#include "Id.h"
#include <QString>
#include <QDate>

struct Note {
    Id id;
    Id parentId;
    int pos;
    int depth;
    QString title;
    QString note;
    QString createdAt;
    QString updatedAt;
    bool markdown;
};

struct FindNote {
    Id id;
    QString title;
};

struct Birthday {
    Id id;
    QDate date;
    QString name;
};
