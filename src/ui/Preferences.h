#pragma once
#include <QDialog>

class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Preferences : public QDialog {
    Q_OBJECT
public:
    Preferences(QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void onFontButtonClicked();
    void onBackupsBrowseButtonClicked();

private:
    QGroupBox* createUiGroupBox();
    QGroupBox* createHotkeyGroupBox();
    QGroupBox* createBackupsGroupBox();

    void readSettings();
    bool writeSettings();

    QComboBox* m_languageComboBox = nullptr;
    QLineEdit* m_fontFamilyLineEdit = nullptr;
    QLineEdit* m_fontSizeLineEdit = nullptr;
    QCheckBox* m_minimizeCheckBox = nullptr;
    QCheckBox* m_hideTrayCheckBox = nullptr;

    QGroupBox* m_hotkeyGroupBox = nullptr;
    QLineEdit* m_hotkeyLineEdit = nullptr;

    QLineEdit* m_backupsLineEdit = nullptr;
};
