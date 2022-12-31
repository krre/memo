#include "MockSettings.h"
#include <ui/Preferences.h>
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
    MockSettings settings;
    settings.general.language = "ru";
    settings.editor.fontFamily = "Ubuntu";
    settings.editor.fontSize = 10;
    settings.general.minimizeOnStartup = true;
    settings.general.hideTrayIcon = true;
    settings.globalHotKey.enabled = true;
    settings.globalHotKey.hotKey = "Ctrl+Alt+M";
    settings.backups.directory = "/home/user/backups";
    settings.server.enabled = true;
    settings.server.key = "123456";
    settings.server.port = 80;

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
    QCOMPARE(preferences.m_keyLineEdit->text(), settings.server.key);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), settings.server.port);
}

void TestPreferences::setOptions() {
    MockSettings settings;
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
    preferences.m_keyLineEdit->setText("123456");
    preferences.m_portLineEdit->setText("80");
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
    QCOMPARE(preferences.m_keyLineEdit->text(), settings.server.key);
    QCOMPARE(preferences.m_portLineEdit->text().toInt(), settings.server.port);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
