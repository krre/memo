#pragma once
#include <QDialog>

class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Options : public QDialog {
    Q_OBJECT
public:
    Options(QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void openFontDialog();

private:
    void readSettings();
    bool writeSettings();

    QComboBox* languageComboBox;
    QLineEdit* fontFamilyLineEdit;
    QLineEdit* fontSizeLineEdit;
    QCheckBox* minimizeCheckBox;
    QCheckBox* hideTrayCheckBox;

    QGroupBox* groupBoxHotkey;
    QLineEdit* hotkeyLineEdit;

    QComboBox* channelComboBox;
    QCheckBox* checkStartupCheckBox;
};
