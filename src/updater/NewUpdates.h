#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class NewUpdates : public QDialog {
    Q_OBJECT

public:
    NewUpdates(const UpdateChecker::Update& update, int size, QWidget* parent = nullptr);
};
