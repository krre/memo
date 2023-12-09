#include <ui/Preferences.h>
#include <core/Settings.h>
#include <QTest>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QGroupBox>

constexpr auto Language = "ru";
constexpr auto FontFamily = "Ubuntu";
constexpr auto FontSize = 10;
constexpr auto MinimizeOnStartup = true;
constexpr auto HideTrayIcon = true;
constexpr auto HotKeyEnabled = true;
constexpr auto HotKey = "Ctrl+Alt+M";
constexpr auto BackupsDirectory = "/home/user/backups";
constexpr auto ServerEnabled = true;
constexpr auto Token = "123456";
constexpr auto Port = 80;
constexpr auto Certificate = "certificate";
constexpr auto PrivateKey = "privateKey";

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
    data.language = Language;
    data.fontFamily = FontFamily;
    data.fontSize = FontSize;
    data.minimizeOnStartup = MinimizeOnStartup;
    data.hideTrayIcon = HideTrayIcon;
    data.hotKeyEnabled = HotKeyEnabled;
    data.hotKey = HotKey;
    data.backupsDirectory = BackupsDirectory;
    data.serverEnabled = ServerEnabled;
    data.token = Token;
    data.port = Port;
    data.certificate = Certificate;
    data.privateKey = PrivateKey;

    Preferences preferences(data);

    QTest::keyClick(&preferences, Qt::Key_Tab); // Cancel button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto languageComboBox = static_cast<QComboBox*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto fontFamilyLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto fontSizeLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab); // Open button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto minimizeCheckBox = static_cast<QCheckBox*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hideTrayCheckBox = static_cast<QCheckBox*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hotkeyGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hotkeyLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto backupsLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab); // Browse... button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto serverGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab); // IP Address text
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto portLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto tokenLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto certificateLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QTest::keyClick(&preferences, Qt::Key_Tab); // Browse... button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto privateKeyEdit = static_cast<QLineEdit*>(preferences.focusWidget());

    QCOMPARE(languageComboBox->currentData(), Language);
    QCOMPARE(fontFamilyLineEdit->text(), FontFamily);
    QCOMPARE(fontSizeLineEdit->text().toInt(), FontSize);
    QCOMPARE(minimizeCheckBox->isChecked(), MinimizeOnStartup);
    QCOMPARE(hideTrayCheckBox->isChecked(), HideTrayIcon);
    QCOMPARE(hotkeyGroupBox->isChecked(), HotKeyEnabled);
    QCOMPARE(hotkeyLineEdit->text(), HotKey);
    QCOMPARE(backupsLineEdit->text(), BackupsDirectory);
    QCOMPARE(serverGroupBox->isChecked(), ServerEnabled);
    QCOMPARE(portLineEdit->text().toInt(), Port);
    QCOMPARE(tokenLineEdit->text(), Token);
    QCOMPARE(certificateLineEdit->text(), Certificate);
    QCOMPARE(privateKeyEdit->text(), PrivateKey);
}

void TestPreferences::setOptions() {
    Preferences::Data inputData;
    inputData.language = Language; // To prevent opening the need restart dialog

    Preferences preferences(inputData);

    QTest::keyClick(&preferences, Qt::Key_Tab); // Cancel button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto languageComboBox = static_cast<QComboBox*>(preferences.focusWidget());
    languageComboBox->setCurrentIndex(languageComboBox->findData(Language));

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto fontFamilyLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    fontFamilyLineEdit->setText(FontFamily);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto fontSizeLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    fontSizeLineEdit->setText(QString::number(FontSize));

    QTest::keyClick(&preferences, Qt::Key_Tab); // Open button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto minimizeCheckBox = static_cast<QCheckBox*>(preferences.focusWidget());
    minimizeCheckBox->setChecked(MinimizeOnStartup);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hideTrayCheckBox = static_cast<QCheckBox*>(preferences.focusWidget());
    hideTrayCheckBox->setChecked(HideTrayIcon);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hotkeyGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());
    hotkeyGroupBox->setChecked(HotKeyEnabled);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto hotkeyLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    hotkeyLineEdit->setText(HotKey);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto backupsLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    backupsLineEdit->setText(BackupsDirectory);

    QTest::keyClick(&preferences, Qt::Key_Tab); // Browse... button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto serverGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());
    serverGroupBox->setChecked(ServerEnabled);

    QTest::keyClick(&preferences, Qt::Key_Tab); // IP Address text
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto portLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    portLineEdit->setText(QString::number(Port));

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto tokenLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    tokenLineEdit->setText(Token);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto certificateLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    certificateLineEdit->setText(Certificate);

    QTest::keyClick(&preferences, Qt::Key_Tab); // Browse... button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto privateKeyEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    privateKeyEdit->setText(PrivateKey);

    preferences.accept();

    Preferences::Data outputData = preferences.data();

    QCOMPARE(outputData.language, Language);
    QCOMPARE(outputData.fontFamily, FontFamily);
    QCOMPARE(outputData.fontSize, FontSize);
    QCOMPARE(outputData.minimizeOnStartup, MinimizeOnStartup);
    QCOMPARE(outputData.hideTrayIcon, HideTrayIcon);
    QCOMPARE(outputData.hotKeyEnabled, HotKeyEnabled);
    QCOMPARE(outputData.hotKey, HotKey);
    QCOMPARE(outputData.backupsDirectory, BackupsDirectory);
    QCOMPARE(outputData.serverEnabled, ServerEnabled);
    QCOMPARE(outputData.port, Port);
    QCOMPARE(outputData.token, Token);
    QCOMPARE(outputData.certificate, Certificate);
    QCOMPARE(outputData.privateKey, PrivateKey);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
