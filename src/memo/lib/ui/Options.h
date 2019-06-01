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

    QComboBox* m_languageComboBox = nullptr;
    QLineEdit* m_fontFamilyLineEdit = nullptr;
    QLineEdit* m_fontSizeLineEdit = nullptr;
    QCheckBox* m_minimizeCheckBox = nullptr;
    QCheckBox* m_hideTrayCheckBox = nullptr;

    QGroupBox* m_groupBoxHotkey = nullptr;
    QLineEdit* m_hotkeyLineEdit = nullptr;

    QComboBox* m_channelComboBox = nullptr;
    QCheckBox* m_checkStartupCheckBox = nullptr;
};
