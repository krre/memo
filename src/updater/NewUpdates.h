#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class NewUpdates : public QDialog {
    Q_OBJECT

public:
    NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent = nullptr);

private:
    QString sizeToMegabyte(int size);
};
