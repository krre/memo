#include "TestSettings.h"
#include "TestHelper.h"
#include <ui/dialog/Preferences.h>
#include <settings/Settings.h>
#include <QTest>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>

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
    using Names = Preferences::Names;

    QCOMPARE(child<QComboBox>(&preferences, Names::LanguageComboBox)->currentData(), Language);
    QCOMPARE(child<QCheckBox>(&preferences, Names::MinimizeCheckBox)->isChecked(), MinimizeOnStartup);
    QCOMPARE(child<QCheckBox>(&preferences, Names::HideTrayCheckBox)->isChecked(), HideTrayIcon);

    QCOMPARE(child<QLineEdit>(&preferences, Names::FontFamilyLineEdit)->text(), FontFamily);
    QCOMPARE(child<QLineEdit>(&preferences, Names::FontSizeLineEdit)->text().toInt(), FontSize);
    QCOMPARE(child<QCheckBox>(&preferences, Names::ShowSymbolsCountCheckBox)->isChecked(), ShowSymbolsCount);

    QCOMPARE(child<QGroupBox>(&preferences, Names::HotkeyGroupBox)->isChecked(), HotKeyEnabled);
    QCOMPARE(child<QLineEdit>(&preferences, Names::HotkeyLineEdit)->text(), HotKeyValue);

    QCOMPARE(child<QCheckBox>(&preferences, Names::RemindBirthdaysCheckBox)->isChecked(), RemindBirthdays);
    QCOMPARE(child<QLineEdit>(&preferences, Names::BackupsLineEdit)->text(), BackupsDirectory);

    QCOMPARE(child<QGroupBox>(&preferences, Names::ServerGroupBox)->isChecked(), ServerEnabled);
    QCOMPARE(child<QLineEdit>(&preferences, Names::PortLineEdit)->text().toInt(), Port);
    QCOMPARE(child<QLineEdit>(&preferences, Names::TokenLineEdit)->text(), Token);
    QCOMPARE(child<QGroupBox>(&preferences, Names::SslGroupBox)->isChecked(), SslEnabled);
    QCOMPARE(child<QLineEdit>(&preferences, Names::CertificateLineEdit)->text(), Certificate);
    QCOMPARE(child<QLineEdit>(&preferences, Names::PrivateKeyEdit)->text(), PrivateKey);
}

void TestPreferences::setPreferences() {
    TestSettings settings;

    // To prevent opening `Restart requred` dialog
    settings.setApplicationLanguage(Language);

    Preferences preferences(&settings);
    using Names = Preferences::Names;

    auto languageComboBox = child<QComboBox>(&preferences, Names::LanguageComboBox);
    languageComboBox->setCurrentIndex(languageComboBox->findData(Language));
    child<QCheckBox>(&preferences, Names::MinimizeCheckBox)->setChecked(MinimizeOnStartup);
    child<QCheckBox>(&preferences, Names::HideTrayCheckBox)->setChecked(HideTrayIcon);

    child<QLineEdit>(&preferences, Names::FontFamilyLineEdit)->setText(FontFamily);
    child<QLineEdit>(&preferences, Names::FontSizeLineEdit)->setText(QString::number(FontSize));
    child<QCheckBox>(&preferences, Names::ShowSymbolsCountCheckBox)->setChecked(ShowSymbolsCount);

    child<QGroupBox>(&preferences, Names::HotkeyGroupBox)->setChecked(HotKeyEnabled);
    child<QLineEdit>(&preferences, Names::HotkeyLineEdit)->setText(HotKeyValue);

    child<QCheckBox>(&preferences, Names::RemindBirthdaysCheckBox)->setChecked(RemindBirthdays);
    child<QLineEdit>(&preferences, Names::BackupsLineEdit)->setText(BackupsDirectory);

    child<QGroupBox>(&preferences, Names::ServerGroupBox)->setChecked(ServerEnabled);
    child<QLineEdit>(&preferences, Names::PortLineEdit)->setText(QString::number(Port));
    child<QLineEdit>(&preferences, Names::TokenLineEdit)->setText(Token);
    child<QGroupBox>(&preferences, Names::SslGroupBox)->setChecked(SslEnabled);
    child<QLineEdit>(&preferences, Names::CertificateLineEdit)->setText(Certificate);
    child<QLineEdit>(&preferences, Names::PrivateKeyEdit)->setText(PrivateKey);

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

QTEST_MAIN(TestPreferences)

#include "TestPreferences.moc"
