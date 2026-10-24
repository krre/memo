#pragma once
#include "StandardDialog.h"

class Settings;
class BrowseLayout;

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

private:
    QGroupBox* createUiGroupBox();
    QGroupBox* createHotkeyGroupBox();
    QGroupBox* createBackupsGroupBox();
    QGroupBox* createServerGroupBox();

    Settings* m_settings = nullptr;

    QComboBox* m_languageComboBox = nullptr;
    QLineEdit* m_fontFamilyLineEdit = nullptr;
    QLineEdit* m_fontSizeLineEdit = nullptr;
    QCheckBox* m_remindBirthdaysCheckBox = nullptr;
    QCheckBox* m_minimizeCheckBox = nullptr;
    QCheckBox* m_hideTrayCheckBox = nullptr;
    QCheckBox* m_showSymbolsCountCheckBox = nullptr;

    QGroupBox* m_hotkeyGroupBox = nullptr;
    QLineEdit* m_hotkeyLineEdit = nullptr;

    BrowseLayout* m_backupsBrowseLayout = nullptr;

    QGroupBox* m_serverGroupBox = nullptr;
    QLineEdit* m_portLineEdit = nullptr;
    QLineEdit* m_tokenLineEdit = nullptr;
    QGroupBox* m_sslGroupBox = nullptr;
    BrowseLayout* m_certificateBrowseLayout = nullptr;
    BrowseLayout* m_privateKeyBrowseLayout = nullptr;

public:
    struct Names {
        static constexpr auto LanguageComboBox = "languageComboBox";
        static constexpr auto FontFamilyLineEdit = "fontFamilyLineEdit";
        static constexpr auto FontSizeLineEdit = "fontSizeLineEdit";
        static constexpr auto MinimizeCheckBox = "minimizeCheckBox";
        static constexpr auto HideTrayCheckBox = "hideTrayCheckBox";
        static constexpr auto RemindBirthdaysCheckBox = "remindBirthdaysCheckBox";
        static constexpr auto ShowSymbolsCountCheckBox = "showSymbolsCountCheckBox";
        static constexpr auto HotkeyGroupBox = "hotkeyGroupBox";
        static constexpr auto HotkeyLineEdit = "hotkeyLineEdit";
        static constexpr auto BackupsLineEdit = "backupsLineEdit";
        static constexpr auto ServerGroupBox = "serverGroupBox";
        static constexpr auto PortLineEdit = "portLineEdit";
        static constexpr auto TokenLineEdit = "tokenLineEdit";
        static constexpr auto SslGroupBox = "sslGroupBox";
        static constexpr auto CertificateLineEdit = "certificateLineEdit";
        static constexpr auto PrivateKeyEdit = "privateKeyEdit";
    };
};
