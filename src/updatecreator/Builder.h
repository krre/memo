#pragma once
#include <QWidget>

class QLineEdit;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(QWidget* parent = nullptr);
    ~Builder();

private slots:
    void selectAppDir();
    void selectWorkspaceDir();

private:
    void readSettings();
    void writeSettings();

    QLineEdit* appLineEdit = nullptr;
    QLineEdit* workspaceLineEdit = nullptr;
};
