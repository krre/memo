#pragma once
#include "core/Globals.h"
#include <QMainWindow>
#include <QSystemTrayIcon>

class QSplitter;

class Settings;
class NoteTaking;
class Editor;
class Database;
class GlobalHotkey;
class HttpServerManager;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

signals:
    void isOpened(bool opened);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNew();
    void onOpen();
    void onExport();
    void onBackup();
    void onClose();
    void onClearRecentFiles();
    void onPreferences();
    void onAbout();
    void onQuit();

    void onNoteChanged(Id id);
    void onEditorFocusLost();

    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onGlobalActivated();

private:
    void readSettings();
    void writeSettings();
    void applyHotSettings();

    void setupSplitter();
    void createActions();
    void createTrayIcon();
    void updateMenuState();

    void loadFile(const QString& filePath);
    void setCurrentFile(const QString& filePath = QString());
    void addRecentFile(const QString& filePath);

    void showErrorDialog(const QString& message);
    QString dateFileName(const QString& name);

    QString m_currentFile;
    QSystemTrayIcon* m_trayIcon = nullptr;
    QMenu* m_trayIconMenu = nullptr;
    QMenu* m_recentFilesMenu = nullptr;
    QAction* m_exportAction = nullptr;
    QAction* m_createBackupAction = nullptr;
    QAction* m_closeAction = nullptr;
    QSplitter* m_splitter = nullptr;

    Settings* m_settings = nullptr;
    NoteTaking* m_notetaking = nullptr;
    Editor* m_editor = nullptr;
    GlobalHotkey* m_globalHotkey = nullptr;
    Database* m_database = nullptr;
    HttpServerManager* m_serverManager = nullptr;

};
