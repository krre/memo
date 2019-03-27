#pragma once
#include <QWidget>

class QLineEdit;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(QWidget* parent = nullptr);

private:
    QLineEdit* appLineEdit = nullptr;
    QLineEdit* workspaceLineEdit = nullptr;
};
