#include <ui/Preferences.h>
#include <core/Settings.h>
#include <QTest>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>

constexpr auto language = "ru";
constexpr auto fontFamily = "Ubuntu";
constexpr auto fontSize = 10;
constexpr auto minimizeOnStartup = true;
constexpr auto hideTrayIcon = true;
constexpr auto hotKeyEnabled = true;
constexpr auto hotKey = "Ctrl+Alt+M";
constexpr auto backupsDirectory = "/home/user/backups";
constexpr auto serverEnabled = true;
constexpr auto token = "123456";
constexpr auto port = 80;
constexpr auto certificate = "certificate";
constexpr auto privateKey = "privateKey";

class TestPreferences : public QObject {
    Q_OBJECT
public:
    TestPreferences();
    ~TestPreferences();

private slots:
    void readOptions();
    void setOptions();
};

TestPreferences::TestPreferences() {

}

TestPreferences::~TestPreferences() {

}

void TestPreferences::readOptions() {
    Preferences::Data data;
    data.language = language;
    data.fontFamily = fontFamily;
    data.fontSize = fontSize;
    data.minimizeOnStartup = minimizeOnStartup;
    data.hideTrayIcon = hideTrayIcon;
    data.hotKeyEnabled = hotKeyEnabled;
    data.hotKey = hotKey;
    data.backupsDirectory = backupsDirectory;
    data.serverEnabled = serverEnabled;
    data.token = token;
    data.port = port;
    data.certificate = certificate;
    data.privateKey = privateKey;

    Preferences preferences(data);

    QCOMPARE(preferences.m_languageComboBox->currentData().toString(), data.language);
    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), data.fontFamily);
    QCOMPARE(preferences.m_fontSizeLineEdit->text().toInt(), data.fontSize);
    QCOMPARE(preferences.m_minimizeCheckBox->isChecked(), data.minimizeOnStartup);
    QCOMPARE(preferences.m_hideTrayCheckBox->isChecked(), data.hideTrayIcon);
    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), data.hotKeyEnabled);
    QCOMPARE(preferences.m_hotkeyLineEdit->text(), data.hotKey);
    QCOMPARE(preferences.m_backupsLineEdit->text(), data.backupsDirectory);
    QCOMPARE(preferences.m_serverGroupBox->isChecked(), data.serverEnabled);
    QCOMPARE(preferences.m_tokenLineEdit->text(), data.token);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), data.port);
    QCOMPARE(preferences.m_certificateLineEdit->text(), data.certificate);
    QCOMPARE(preferences.m_privateKeyEdit->text(), data.privateKey);
}

void TestPreferences::setOptions() {
    Preferences::Data inputData;
    inputData.language = language; // To prevent opening the need restart dialog

    Preferences preferences(inputData);

    int index = preferences.m_languageComboBox->findData(language);
    preferences.m_languageComboBox->setCurrentIndex(index);
    preferences.m_fontFamilyLineEdit->setText(fontFamily);
    preferences.m_fontSizeLineEdit->setText(QString::number(fontSize));
    preferences.m_minimizeCheckBox->setChecked(minimizeOnStartup);
    preferences.m_hideTrayCheckBox->setChecked(hideTrayIcon);
    preferences.m_hotkeyGroupBox->setChecked(hotKeyEnabled);
    preferences.m_hotkeyLineEdit->setText(hotKey);
    preferences.m_backupsLineEdit->setText(backupsDirectory);
    preferences.m_serverGroupBox->setChecked(serverEnabled);
    preferences.m_tokenLineEdit->setText(token);
    preferences.m_portLineEdit->setText(QString::number(port));
    preferences.m_certificateLineEdit->setText(certificate);
    preferences.m_privateKeyEdit->setText(privateKey);
    preferences.accept();

    Preferences::Data outputData = preferences.data();

    QCOMPARE(preferences.m_languageComboBox->currentData().toString(), outputData.language);
    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), outputData.fontFamily);
    QCOMPARE(preferences.m_fontSizeLineEdit->text().toInt(), outputData.fontSize);
    QCOMPARE(preferences.m_minimizeCheckBox->isChecked(), outputData.minimizeOnStartup);
    QCOMPARE(preferences.m_hideTrayCheckBox->isChecked(), outputData.hideTrayIcon);
    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), outputData.hotKeyEnabled);
    QCOMPARE(preferences.m_hotkeyLineEdit->text(), outputData.hotKey);
    QCOMPARE(preferences.m_backupsLineEdit->text(), outputData.backupsDirectory);
    QCOMPARE(preferences.m_serverGroupBox->isChecked(), outputData.serverEnabled);
    QCOMPARE(preferences.m_tokenLineEdit->text(), outputData.token);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), outputData.port);
    QCOMPARE(preferences.m_certificateLineEdit->text(), outputData.certificate);
    QCOMPARE(preferences.m_privateKeyEdit->text(), outputData.privateKey);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
