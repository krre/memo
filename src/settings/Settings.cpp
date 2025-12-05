#include "Settings.h"

namespace Application {
    constexpr auto Language = "Application/language";
    constexpr auto FilePath = "Application/filePath";
    constexpr auto MinimizeOnStartup = "Application/minimizeOnStartup";
    constexpr auto HideTrayIcon = "Application/hideTrayIcon";
}

namespace MainWindow {
    constexpr auto Geometry = "MainWindow/geometry";
    constexpr auto State = "MainWindow/state";
    constexpr auto Splitter = "MainWindow/splitter";
}

namespace Birthdays {
    constexpr auto Geometry = "Birthdays/geometry";
    constexpr auto Remind = "Birthdays/remind";
}

namespace Backups {
    constexpr auto Directory = "Backups/directory";
}

namespace Editor {
    constexpr auto FontFamily = "Editor/fontFamily";
    constexpr auto FontSize = "Editor/fontSize";
    constexpr auto ShowSymbolsCount = "Editor/showSymbolsCount";
}

namespace GlobalHotkey {
    constexpr auto Enabled = "GlobalHotkey/enabled";
    constexpr auto Value = "GlobalHotkey/value";
}

namespace RecentFiles {
    constexpr auto File = "RecentFiles/file";
}

namespace Server {
    constexpr auto Enabled = "Server/enabled";
    constexpr auto Token = "Server/token";
    constexpr auto Port = "Server/port";
    constexpr auto SslEnabled = "Server/sslEnabled";
    constexpr auto Certificate = "Server/certificate";
    constexpr auto PrivateKey = "Server/privateKey";
}

void Settings::setApplicationLanguage(const QString& lang) {
    setValue(Application::Language, lang);
}

QString Settings::applicationLanguage() const {
    return value(Application::Language).toString();
}

void Settings::setApplicationFilePath(const QString& filePath) {
    setValue(Application::FilePath, filePath);
}

QString Settings::applicationFilePath() const {
    return value(Application::FilePath).toString();
}

void Settings::setApplicationMinimizeOnStartup(bool minimizeOnStartup) {
    setValue(Application::MinimizeOnStartup, minimizeOnStartup);
}

bool Settings::applicationMinimizeOnStartup() const {
    return value(Application::MinimizeOnStartup, true).toBool();
}

void Settings::setApplicationHideTrayIcon(bool hideTrayIcon) {
    setValue(Application::HideTrayIcon, hideTrayIcon);
}

bool Settings::applicationHideTrayIcon() const {
    return value(Application::HideTrayIcon).toBool();
}

void Settings::setMainWindowGeometry(const QByteArray& geometry) {
    setValue(MainWindow::Geometry, geometry);
}

QByteArray Settings::mainWindowGeometry() const {
    return value(MainWindow::Geometry).toByteArray();
}

void Settings::setMainWindowState(const QByteArray& state) {
    setValue(MainWindow::State, state);
}

QByteArray Settings::mainWindowState() const {
    return value(MainWindow::State).toByteArray();
}

void Settings::setMainWindowSplitter(const QByteArray& splitter) {
    setValue(MainWindow::Splitter, splitter);
}

QByteArray Settings::mainWindowSplitter() const {
    return value(MainWindow::Splitter).toByteArray();
}

void Settings::setBirthdaysGeometry(const QByteArray& geometry) {
    setValue(Birthdays::Geometry, geometry);
}

QByteArray Settings::birthdaysGeometry() const {
    return value(Birthdays::Geometry).toByteArray();
}

void Settings::setBirthdaysRemind(bool remind) {
    setValue(Birthdays::Remind, remind);
}

bool Settings::birthdaysRemind() const {
    return value(Birthdays::Remind, true).toBool();
}

void Settings::setBackupsDirectory(const QString& directory) {
    setValue(Backups::Directory, directory);
}

QString Settings::backupsDirectory() const {
    return value(Backups::Directory).toString();
}

void Settings::setEditorFontFamily(const QString& fontFamily) {
    setValue(Editor::FontFamily, fontFamily);
}

QString Settings::editorFontFamily() const {
    return value(Editor::FontFamily).toString();
}

void Settings::setEditorFontSize(int fontSize) {
    setValue(Editor::FontSize, fontSize);
}

int Settings::editorFontSize() const {
    return value(Editor::FontSize).toInt();
}

void Settings::setEditorShowSymbolsCount(bool show) {
    setValue(Editor::ShowSymbolsCount, show);
}

int Settings::editorShowSymbolsCount() const {
    return value(Editor::ShowSymbolsCount).toBool();
}

void Settings::setGlobalHotkeyEnabled(bool enabled) {
    setValue(GlobalHotkey::Enabled, enabled);
}

bool Settings::globalHotkeyEnabled() const {
    return value(GlobalHotkey::Enabled, true).toBool();
}

void Settings::setGlobalHotkeyValue(const QString& value) {
    setValue(GlobalHotkey::Value, value);
}

QString Settings::globalHotkeyValue() const {
    return value(GlobalHotkey::Value, "Ctrl+Alt+M").toString();
}

void Settings::setRecentFiles(const QStringList& recentFiles) {
    QVariantList list;

    for (const auto& file : recentFiles) {
        list.append(file);
    }

    setList(RecentFiles::File, list);
}

QStringList Settings::recentFiles() const {
    QStringList result;
    const QVariantList recentFiles = list(RecentFiles::File);

    for (const auto& value : recentFiles) {
        result.append(value.toString());
    }

    return result;
}

void Settings::setServerEnabled(bool enabled) {
    setValue(Server::Enabled, enabled);
}

bool Settings::serverEnabled() const {
    return value(Server::Enabled).toBool();
}

void Settings::setServerToken(const QString& token) {
    setValue(Server::Token, token);
}

QString Settings::serverToken() const {
    return value(Server::Token).toString();
}

void Settings::setServerPort(int port) {
    setValue(Server::Port, port);
}

int Settings::serverPort() const {
    return value(Server::Port).toInt();
}

void Settings::setServerSslEnabled(bool enabled) {
    setValue(Server::SslEnabled, enabled);
}

bool Settings::serverSslEnabled() const {
    return value(Server::SslEnabled).toBool();
}

void Settings::setServerCertificate(const QString& certificate) {
    setValue(Server::Certificate, certificate);
}

QString Settings::serverCertificate() const {
    return value(Server::Certificate).toString();
}

void Settings::setServerPrivateKey(const QString& privateKey) {
    setValue(Server::PrivateKey, privateKey);
}

QString Settings::serverPrivateKey() const {
    return value(Server::PrivateKey).toString();
}
