#pragma once
#include "core/Globals.h"
#include <QMainWindow>

class QSplitter;

class RecentFilesMenu;
class NoteTaking;
class TrayIcon;
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

public slots:
    void quit();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNew();
    void onOpen();
    void onExport();
    void onBackup();
    void onClose();
    void onPreferences();
    void onFind();
    void onFindNext();
    void onFindPrevious();
    void onAbout();

    void onNoteChanged(Id id);
    void onEditorFocusLost();

    void onGlobalActivated();

private slots:
    void loadFile(const QString& filePath);

private:
    void readSettings();
    void writeSettings();
    void applyHotSettings();

    void setupSplitter();
    void createActions();

    void setCurrentFile(const QString& filePath = QString());

    void showErrorDialog(const QString& message);
    QString dateFileName(const QString& name);

    QString m_currentFile;
    TrayIcon* m_trayIcon = nullptr;
    RecentFilesMenu* m_recentFilesMenu = nullptr;
    QSplitter* m_splitter = nullptr;

    NoteTaking* m_notetaking = nullptr;
    Editor* m_editor = nullptr;
    GlobalHotkey* m_globalHotkey = nullptr;
    Database* m_database = nullptr;
    HttpServerManager* m_serverManager = nullptr;
    QString m_findText;

    QAction* m_findNextAction = nullptr;
    QAction* m_findPreviousAction = nullptr;

};
