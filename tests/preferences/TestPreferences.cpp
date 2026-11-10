#include "TestSettings.h"
#include <ui/dialog/Preferences.h>
#include <ui/dialog/BrowseLayout.h>
#include <settings/Settings.h>
#include <QTest>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>

constexpr auto Language = "ru";
constexpr auto MinimizeOnStartup = true;
constexpr auto HideTrayIcon = true;

constexpr auto FontFamily = "Ubuntu";
constexpr auto FontSize = 10;
constexpr auto ShowSymbolsCount = true;

constexpr auto HotKeyEnabled = true;
constexpr auto HotKeyValue = "Ctrl+Alt+M";

constexpr auto RemindBirthdays = true;
constexpr auto BackupsDirectory = "/home/user/backups";

constexpr auto ServerEnabled = true;
constexpr auto Port = 80;
constexpr auto Token = "123456";
constexpr auto SslEnabled = true;
constexpr auto Certificate = "certificate";
constexpr auto PrivateKey = "privateKey";

class TestPreferences : public QObject {
    Q_OBJECT
private slots:
    void readPreferences();
    void setPreferences();
    void applyActions();
};

void TestPreferences::readPreferences() {
    TestSettings settings;

    settings.setApplicationLanguage(Language);
    settings.setApplicationMinimizeOnStartup(MinimizeOnStartup);
    settings.setApplicationHideTrayIcon(HideTrayIcon);

    settings.setEditorFontFamily(FontFamily);
    settings.setEditorFontSize(FontSize);
    settings.setEditorShowSymbolsCount(ShowSymbolsCount);

    settings.setGlobalHotkeyEnabled(HotKeyEnabled);
    settings.setGlobalHotkeyValue(HotKeyValue);

    settings.setBirthdaysRemind(RemindBirthdays);
    settings.setBackupsDirectory(BackupsDirectory);

    settings.setServerEnabled(ServerEnabled);
    settings.setServerPort(Port);
    settings.setServerToken(Token);
    settings.setServerSslEnabled(SslEnabled);
    settings.setServerCertificate(Certificate);
    settings.setServerPrivateKey(PrivateKey);

    Preferences preferences(&settings);
    QCOMPARE(preferences.m_languageComboBox->currentData(), Language);
    QCOMPARE(preferences.m_minimizeCheckBox->isChecked(), MinimizeOnStartup);
    QCOMPARE(preferences.m_hideTrayCheckBox->isChecked(), HideTrayIcon);

    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), FontFamily);
    QCOMPARE(preferences.m_fontSizeLineEdit->text().toInt(), FontSize);
    QCOMPARE(preferences.m_showSymbolsCountCheckBox->isChecked(), ShowSymbolsCount);

    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), HotKeyEnabled);
    QCOMPARE(preferences.m_hotkeyLineEdit->text(), HotKeyValue);

    QCOMPARE(preferences.m_remindBirthdaysCheckBox->isChecked(), RemindBirthdays);
    QCOMPARE(preferences.m_backupsBrowseLayout->text(), BackupsDirectory);

    QCOMPARE(preferences.m_serverGroupBox->isChecked(), ServerEnabled);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), Port);
    QCOMPARE(preferences.m_tokenLineEdit->text(), Token);
    QCOMPARE(preferences.m_sslGroupBox->isChecked(), SslEnabled);
    QCOMPARE(preferences.m_certificateBrowseLayout->text(), Certificate);
    QCOMPARE(preferences.m_privateKeyBrowseLayout->text(), PrivateKey);
}

void TestPreferences::setPreferences() {
    TestSettings settings;

    // To prevent opening `Restart requred` dialog
    settings.setApplicationLanguage(Language);

    Preferences preferences(&settings);

    preferences.m_languageComboBox->setCurrentIndex(preferences.m_languageComboBox->findData(Language));
    preferences.m_minimizeCheckBox->setChecked(MinimizeOnStartup);
    preferences.m_hideTrayCheckBox->setChecked(HideTrayIcon);

    preferences.m_fontFamilyLineEdit->setText(FontFamily);
    preferences.m_fontSizeLineEdit->setText(QString::number(FontSize));
    preferences.m_showSymbolsCountCheckBox->setChecked(ShowSymbolsCount);

    preferences.m_hotkeyGroupBox->setChecked(HotKeyEnabled);
    preferences.m_hotkeyLineEdit->setText(HotKeyValue);

    preferences.m_remindBirthdaysCheckBox->setChecked(RemindBirthdays);
    preferences.m_backupsBrowseLayout->setText(BackupsDirectory);

    preferences.m_serverGroupBox->setChecked(ServerEnabled);
    preferences.m_portLineEdit->setText(QString::number(Port));
    preferences.m_tokenLineEdit->setText(Token);
    preferences.m_sslGroupBox->setChecked(SslEnabled);
    preferences.m_certificateBrowseLayout->setText(Certificate);
    preferences.m_privateKeyBrowseLayout->setText(PrivateKey);

    preferences.accept();

    QCOMPARE(settings.applicationLanguage(), Language);
    QCOMPARE(settings.applicationMinimizeOnStartup(), MinimizeOnStartup);
    QCOMPARE(settings.applicationHideTrayIcon(), HideTrayIcon);

    QCOMPARE(settings.editorFontFamily(), FontFamily);
    QCOMPARE(settings.editorFontSize(), FontSize);
    QCOMPARE(settings.editorShowSymbolsCount(), ShowSymbolsCount);

    QCOMPARE(settings.globalHotkeyEnabled(), HotKeyEnabled);
    QCOMPARE(settings.globalHotkeyValue(), HotKeyValue);

    QCOMPARE(settings.birthdaysRemind(), RemindBirthdays);
    QCOMPARE(settings.backupsDirectory(), BackupsDirectory);

    QCOMPARE(settings.serverEnabled(), ServerEnabled);
    QCOMPARE(settings.serverPort(), Port);
    QCOMPARE(settings.serverToken(), Token);
    QCOMPARE(settings.serverSslEnabled(), SslEnabled);
    QCOMPARE(settings.serverCertificate(), Certificate);
    QCOMPARE(settings.serverPrivateKey(), PrivateKey);
}

void TestPreferences::applyActions() {
    TestSettings settings;
    Preferences preferences(&settings);

    preferences.m_fontFamilyLineEdit->setText("Font family");
    preferences.m_fontSizeLineEdit->setText("10");

    auto resetFontButton = preferences.findChild<QPushButton*>("resetFontButton");
    resetFontButton->click();

    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), QString());
    QCOMPARE(preferences.m_fontSizeLineEdit->text(), QString());
}

QTEST_MAIN(TestPreferences)

#include "TestPreferences.moc"
