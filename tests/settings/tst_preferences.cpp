#include <ui/Preferences.h>
#include <core/Settings.h>
#include <QTest>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>

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
    Settings settings;
    settings.general.language = "ru";
    settings.editor.fontFamily = "Ubuntu";
    settings.editor.fontSize = 10;
    settings.general.minimizeOnStartup = true;
    settings.general.hideTrayIcon = true;
    settings.globalHotKey.enabled = true;
    settings.globalHotKey.hotKey = "Ctrl+Alt+M";
    settings.backups.directory = "/home/user/backups";
    settings.server.enabled = true;
    settings.server.token = "123456";
    settings.server.port = 80;
    settings.server.certificate = "certificate";
    settings.server.privateKey = "privateKey";

    Preferences preferences(&settings);

    QCOMPARE(preferences.m_languageComboBox->currentData().toString(), settings.general.language);
    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), settings.editor.fontFamily);
    QCOMPARE(preferences.m_fontSizeLineEdit->text().toInt(), settings.editor.fontSize);
    QCOMPARE(preferences.m_minimizeCheckBox->isChecked(), settings.general.minimizeOnStartup);
    QCOMPARE(preferences.m_hideTrayCheckBox->isChecked(), settings.general.hideTrayIcon);
    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), settings.globalHotKey.enabled);
    QCOMPARE(preferences.m_hotkeyLineEdit->text(), settings.globalHotKey.hotKey);
    QCOMPARE(preferences.m_backupsLineEdit->text(), settings.backups.directory);
    QCOMPARE(preferences.m_serverGroupBox->isChecked(), settings.server.enabled);
    QCOMPARE(preferences.m_tokenLineEdit->text(), settings.server.token);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), settings.server.port);
    QCOMPARE(preferences.m_certificateLineEdit->text(), settings.server.certificate);
    QCOMPARE(preferences.m_privateKeyEdit->text(), settings.server.privateKey);
}

void TestPreferences::setOptions() {
    Settings settings;
    settings.general.language = "ru"; // To prevent opening the need restart dialog
    Preferences preferences(&settings);

    int index = preferences.m_languageComboBox->findData("ru");
    preferences.m_languageComboBox->setCurrentIndex(index);
    preferences.m_fontFamilyLineEdit->setText("Ubuntu");
    preferences.m_fontSizeLineEdit->setText("10");
    preferences.m_minimizeCheckBox->setChecked(true);
    preferences.m_hideTrayCheckBox->setChecked(true);
    preferences.m_hotkeyGroupBox->setChecked(true);
    preferences.m_hotkeyLineEdit->setText("Ctrl+Alt+M");
    preferences.m_backupsLineEdit->setText("/home/user/backups");
    preferences.m_serverGroupBox->setChecked(true);
    preferences.m_tokenLineEdit->setText("123456");
    preferences.m_portLineEdit->setText("80");
    preferences.m_certificateLineEdit->setText("certificate");
    preferences.m_privateKeyEdit->setText("privateKey");
    preferences.accept();

    QCOMPARE(preferences.m_languageComboBox->currentData().toString(), settings.general.language);
    QCOMPARE(preferences.m_fontFamilyLineEdit->text(), settings.editor.fontFamily);
    QCOMPARE(preferences.m_fontSizeLineEdit->text().toInt(), settings.editor.fontSize);
    QCOMPARE(preferences.m_minimizeCheckBox->isChecked(), settings.general.minimizeOnStartup);
    QCOMPARE(preferences.m_hideTrayCheckBox->isChecked(), settings.general.hideTrayIcon);
    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), settings.globalHotKey.enabled);
    QCOMPARE(preferences.m_hotkeyGroupBox->isChecked(), settings.globalHotKey.enabled);
    QCOMPARE(preferences.m_hotkeyLineEdit->text(), settings.globalHotKey.hotKey);
    QCOMPARE(preferences.m_backupsLineEdit->text(), settings.backups.directory);
    QCOMPARE(preferences.m_serverGroupBox->isChecked(), settings.server.enabled);
    QCOMPARE(preferences.m_tokenLineEdit->text(), settings.server.token);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), settings.server.port);
    QCOMPARE(preferences.m_certificateLineEdit->text(), settings.server.certificate);
    QCOMPARE(preferences.m_privateKeyEdit->text(), settings.server.privateKey);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
