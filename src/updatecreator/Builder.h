#pragma once
#include <QWidget>

class QLineEdit;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(QWidget* parent = nullptr);
    ~Builder();

private slots:
    void selectDirectory();

private:
    void createAppDirWidgets();
    void createFilesWidgets();

    void readSettings();
    void writeSettings();

    QLineEdit* appDirLineEdit = nullptr;
};
