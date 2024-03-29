#pragma once
#include "StandardDialog.h"

class Settings;

class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Preferences : public StandardDialog {
    Q_OBJECT
public:
    Preferences(Settings* settings, QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void onFontButtonClicked();
    void onBackupsBrowseButtonClicked();
    void onCertificateBrowseButtonClicked();
    void onPrivateKeyBrowseButtonClicked();

private:
    QGroupBox* createUiGroupBox();
    QGroupBox* createHotkeyGroupBox();
    QGroupBox* createBackupsGroupBox();
    QGroupBox* createServerGroupBox();

    Settings* m_settings = nullptr;

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
    QLineEdit* m_tokenLineEdit = nullptr;
    QLineEdit* m_certificateLineEdit = nullptr;
    QLineEdit* m_privateKeyEdit = nullptr;
};
