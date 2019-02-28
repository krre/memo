#pragma once
#include <QDialog>

class QComboBox;
class QCheckBox;

class Options : public QDialog {
    Q_OBJECT

public:
    Options(QWidget* parent = nullptr);

public slots:
    void accept() override;

private:
    void readSettings();
    bool writeSettings();

    QComboBox* languageComboBox;
    QCheckBox* minimizeCheckBox;
};
