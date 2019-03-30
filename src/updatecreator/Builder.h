#pragma once
#include <QWidget>

class QLineEdit;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(QWidget* parent = nullptr);
    ~Builder();

private:
    void createFilesWidgets();

    void readSettings();
    void writeSettings();
};
