#pragma once
#include "StandardDialog.h"

class Settings;

class QComboBox;
class QLineEdit;
class QCheckBox;
class QGroupBox;

class Preferences : public StandardDialog {
    Q_OBJECT
    friend class TestPreferences;
public:
    struct Data {
        QString language;
        QString backupsDirectory;

        QString fontFamily;
        int fontSize;

        bool hideTrayIcon;
        bool minimizeOnStartup;

        bool hotKeyEnabled;
        QString hotKey;

        bool serverEnabled;
        QString token;
        int port;
        QString certificate;
        QString privateKey;
    };

    Preferences(const Data& data, QWidget* parent = nullptr);
    Data data() const;

public slots:
    void accept() override;

private slots:
    void onFontButtonClicked();
    void onBackupsBrowseButtonClicked();
    void onCertificateBrowseButtonClicked();
    void onPrivateKeyBrowseButtonClicked();

private:
    QGroupBox* createUiGroupBox(const Data& data);
    QGroupBox* createHotkeyGroupBox(const Data& data);
    QGroupBox* createBackupsGroupBox(const Data& data);
    QGroupBox* createServerGroupBox(const Data& data);

    QString m_language;

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
