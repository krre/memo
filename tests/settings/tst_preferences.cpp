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
constexpr auto HotKey = "Ctrl+Alt+M";
constexpr auto BackupsDirectory = "/home/user/backups";
constexpr auto ServerEnabled = true;
constexpr auto Token = "123456";
constexpr auto Port = 80;
constexpr auto Certificate = "certificate";
constexpr auto PrivateKey = "privateKey";

class TestSettings : public Settings {

public:
    void setGeneral(const General& general) override {
        m_general = general;
    }

    General general() const override {
        return m_general;
    }

    void setMainWindow(const MainWindow& mainWindow) override {
        m_mainWindow = mainWindow;
    }

    MainWindow mainWindow() const override {
        return m_mainWindow;
    }

    bool containsGeometry() const override {
        return true;
    }

    void setBackups(const Backups& backups) override {
        m_backups = backups;
    }

    Backups backups() const override {
        return m_backups;
    }

    void setEditor(const Editor& editor) override {
        m_editor = editor;
    }

    Editor editor() const override {
        return m_editor;
    }

    void setGlobalHotkey(const GlobalHotkey& globalHotkey) override {
        m_globalHotkey = globalHotkey;
    }

    GlobalHotkey globalHotkey() const override {
        return m_globalHotkey;
    }

    void setRecent(const Recent& recent) override {
        m_recent = recent;
    }

    Recent recent() const override {
        return m_recent;
    }

    void setServer(const Server& server) override {
        m_server = server;
    }

    Server server() const override {
        return m_server;
    }

private:
    General m_general;
    MainWindow m_mainWindow;
    Backups m_backups;
    Editor m_editor;
    GlobalHotkey m_globalHotkey;
    Recent m_recent;
    Server m_server;
};

class TestPreferences : public QObject {
    Q_OBJECT
private slots:
    void readOptions();
    void setOptions();
};

void TestPreferences::readOptions() {
    TestSettings settings;

    TestSettings::General general;
    general.language = Language;
    general.minimizeOnStartup = MinimizeOnStartup;
    general.hideTrayIcon = HideTrayIcon;
    settings.setGeneral(general);

    TestSettings::Editor editor;
    editor.fontFamily = FontFamily;
    editor.fontSize = FontSize;
    settings.setEditor(editor);

    TestSettings::GlobalHotkey globalHotkey;
    globalHotkey.enabled = HotKeyEnabled;
    globalHotkey.hotkey = HotKey;
    settings.setGlobalHotkey(globalHotkey);

    TestSettings::Backups backups;
    backups.directory = BackupsDirectory;
    settings.setBackups(backups);

    TestSettings::Server server;
    server.enabled = ServerEnabled;
    server.token = Token;
    server.port = Port;
    server.certificate = Certificate;
    server.privateKey = PrivateKey;
    settings.setServer(server);

    Preferences preferences(&settings);

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
    TestSettings settings;

    // To prevent opening the need restart dialog
    TestSettings::General general;
    general.language = Language;
    settings.setGeneral(general);

    Preferences preferences(&settings);

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

    QCOMPARE(settings.general().language, Language);
    QCOMPARE(settings.general().minimizeOnStartup, MinimizeOnStartup);
    QCOMPARE(settings.general().hideTrayIcon, HideTrayIcon);
    QCOMPARE(settings.editor().fontFamily, FontFamily);
    QCOMPARE(settings.editor().fontSize, FontSize);
    QCOMPARE(settings.globalHotkey().enabled, HotKeyEnabled);
    QCOMPARE(settings.globalHotkey().hotkey, HotKey);
    QCOMPARE(settings.backups().directory, BackupsDirectory);
    QCOMPARE(settings.server().enabled, ServerEnabled);
    QCOMPARE(settings.server().port, Port);
    QCOMPARE(settings.server().token, Token);
    QCOMPARE(settings.server().certificate, Certificate);
    QCOMPARE(settings.server().privateKey, PrivateKey);
}

QTEST_MAIN(TestPreferences)

#include "tst_preferences.moc"
