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
    Preferences::Data data;
    data.language = "ru";
    data.fontFamily = "Ubuntu";
    data.fontSize = 10;
    data.minimizeOnStartup = true;
    data.hideTrayIcon = true;
    data.hotKeyEnabled = true;
    data.hotKey = "Ctrl+Alt+M";
    data.backupsDirectory = "/home/user/backups";
    data.serverEnabled = true;
    data.token = "123456";
    data.port = 80;
    data.certificate = "certificate";
    data.privateKey = "privateKey";

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
    inputData.language = "ru"; // To prevent opening the need restart dialog

    Preferences preferences(inputData);

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
