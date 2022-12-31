#pragma once
#include <QDialog>

class ISettings;

class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Preferences : public QDialog {
    Q_OBJECT
    friend class TestPreferences;
public:
    Preferences(ISettings* settings, QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void onFontButtonClicked();
    void onBackupsBrowseButtonClicked();

private:
    QGroupBox* createUiGroupBox();
    QGroupBox* createHotkeyGroupBox();
    QGroupBox* createBackupsGroupBox();
    QGroupBox* createServerGroupBox();

    void readSettings();
    bool writeSettings();

    ISettings* m_settings = nullptr;

    QComboBox* m_languageComboBox = nullptr;
    QLineEdit* m_fontFamilyLineEdit = nullptr;
    QLineEdit* m_fontSizeLineEdit = nullptr;
    QCheckBox* m_minimizeCheckBox = nullptr;
    QCheckBox* m_hideTrayCheckBox = nullptr;

    QGroupBox* m_hotkeyGroupBox = nullptr;
    QLineEdit* m_hotkeyLineEdit = nullptr;

    QLineEdit* m_backupsLineEdit = nullptr;

    QGroupBox* m_serverGroupBox = nullptr;
    QLineEdit* m_portLineEdit = nullptr;
    QLineEdit* m_keyLineEdit = nullptr;
};
