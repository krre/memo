#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class NewUpdate : public QDialog {
    Q_OBJECT

public:
    NewUpdate(const UpdateChecker::Update& update, int size, QWidget* parent = nullptr);
};
