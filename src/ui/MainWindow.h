#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>
#include <core/Globals.h>

class QSplitter;
class Outliner;
class Editor;
class Database;
class GlobalHotkey;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNew();
    void onOpen();
    void onExport();
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

    QSystemTrayIcon* m_trayIcon = nullptr;
    QMenu* m_trayIconMenu = nullptr;
    QMenu* m_recentFilesMenu = nullptr;
    QAction* m_exportAction = nullptr;
    QAction* m_closeAction = nullptr;

    QSplitter* m_splitter = nullptr;
    Outliner* m_outliner = nullptr;
    Editor* m_editor = nullptr;
    GlobalHotkey* m_globalHotkey = nullptr;

    Database* m_database = nullptr;
    QString m_currentFile;
};
