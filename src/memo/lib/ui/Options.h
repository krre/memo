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

    QComboBox* m_languageComboBox;
    QLineEdit* m_fontFamilyLineEdit;
    QLineEdit* m_fontSizeLineEdit;
    QCheckBox* m_minimizeCheckBox;
    QCheckBox* m_hideTrayCheckBox;

    QGroupBox* m_groupBoxHotkey;
    QLineEdit* m_hotkeyLineEdit;

    QComboBox* m_channelComboBox;
    QCheckBox* m_checkStartupCheckBox;
};
