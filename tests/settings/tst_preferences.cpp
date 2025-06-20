#include <ui/dialog/Preferences.h>
#include <settings/Settings.h>
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
constexpr auto HotKeyValue = "Ctrl+Alt+M";
constexpr auto BackupsDirectory = "/home/user/backups";
constexpr auto ServerEnabled = true;
constexpr auto Token = "123456";
constexpr auto Port = 80;
constexpr auto SslEnabled = true;
constexpr auto Certificate = "certificate";
constexpr auto PrivateKey = "privateKey";

class TestSettings : public Settings {

protected:
    void setValue(const QString& key, const QVariant& value) override {
        settings[key] = value;
    }

    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const override {
        return settings.contains(key) ? settings[key] : defaultValue;
    }

    void setList(const QString& key, const QVariantList& list) override {
        settings[key] = list;
    }

    QVariantList list(const QString& key) const override {
        return settings[key].toList();
    }

private:
    QHash<QString, QVariant> settings;
};

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

    settings.setGlobalHotkeyEnabled(HotKeyEnabled);
    settings.setGlobalHotkeyValue(HotKeyValue);

    settings.setBackupsDirectory(BackupsDirectory);

    settings.setServerEnabled(ServerEnabled);
    settings.setServerToken(Token);
    settings.setServerPort(Port);
    settings.setServerSslEnabled(SslEnabled);
    settings.setServerCertificate(Certificate);
    settings.setServerPrivateKey(PrivateKey);

    Preferences preferences(&settings);

    QTest::keyClick(&preferences, Qt::Key_Tab); // OK button
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
    auto sslGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());

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
    QCOMPARE(hotkeyLineEdit->text(), HotKeyValue);
    QCOMPARE(backupsLineEdit->text(), BackupsDirectory);
    QCOMPARE(serverGroupBox->isChecked(), ServerEnabled);
    QCOMPARE(portLineEdit->text().toInt(), Port);
    QCOMPARE(tokenLineEdit->text(), Token);
    QCOMPARE(sslGroupBox->isChecked(), SslEnabled);
    QCOMPARE(certificateLineEdit->text(), Certificate);
    QCOMPARE(privateKeyEdit->text(), PrivateKey);
}

void TestPreferences::setPreferences() {
    TestSettings settings;

    // To prevent opening the need restart dialog
    settings.setApplicationLanguage(Language);

    Preferences preferences(&settings);

    QTest::keyClick(&preferences, Qt::Key_Tab); // OK button
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
    hotkeyLineEdit->setText(HotKeyValue);

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
    auto sslGroupBox = static_cast<QGroupBox*>(preferences.focusWidget());
    sslGroupBox->setChecked(SslEnabled);

    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto certificateLineEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    certificateLineEdit->setText(Certificate);

    QTest::keyClick(&preferences, Qt::Key_Tab); // Browse... button
    QTest::keyClick(&preferences, Qt::Key_Tab);
    auto privateKeyEdit = static_cast<QLineEdit*>(preferences.focusWidget());
    privateKeyEdit->setText(PrivateKey);

    preferences.accept();

    QCOMPARE(settings.applicationLanguage(), Language);
    QCOMPARE(settings.applicationMinimizeOnStartup(), MinimizeOnStartup);
    QCOMPARE(settings.applicationHideTrayIcon(), HideTrayIcon);
    QCOMPARE(settings.editorFontFamily(), FontFamily);
    QCOMPARE(settings.editorFontSize(), FontSize);
    QCOMPARE(settings.globalHotkeyEnabled(), HotKeyEnabled);
    QCOMPARE(settings.globalHotkeyValue(), HotKeyValue);
    QCOMPARE(settings.backupsDirectory(), BackupsDirectory);
    QCOMPARE(settings.serverEnabled(), ServerEnabled);
    QCOMPARE(settings.serverPort(), Port);
    QCOMPARE(settings.serverToken(), Token);
    QCOMPARE(settings.serverSslEnabled(), SslEnabled);
    QCOMPARE(settings.serverCertificate(), Certificate);
    QCOMPARE(settings.serverPrivateKey(), PrivateKey);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
