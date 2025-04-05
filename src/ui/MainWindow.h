#pragma once
#include "core/Id.h"
#include <QMainWindow>

class FileSettings;
class RecentFilesMenu;
class NoteTaking;
class TrayIcon;
class Editor;
class Navigation;
class Database;
class GlobalHotkey;
class HttpServerManager;

class QSplitter;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

signals:
    void isOpened(bool opened);
    void isEdited(bool edited);

public slots:
    void quit();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void createFile();
    void open();
    void exportAll();
    void backup();
    void closeFile();
    void showPreferences();
    void find();
    void findInAllNotes();
    void findNext();
    void findPrevious();
    void showBirthdays();
    void about();

    void onEditorFocusLost();
    void onGlobalActivated();

    void loadFile(const QString& filePath);

    void openNote(Id id);
    void closeNote();

private:
    void readSettings();
    void writeSettings();
    void applyHotSettings();

    void setupSplitter();
    void createActions();

    void setCurrentFile(const QString& filePath = QString());

    void showErrorDialog(const QString& message);
    QString dateFileName(const QString& name);

    QScopedPointer<FileSettings> m_fileSettings;

    QString m_currentFile;
    TrayIcon* m_trayIcon = nullptr;
    RecentFilesMenu* m_recentFilesMenu = nullptr;
    QSplitter* m_splitter = nullptr;

    NoteTaking* m_notetaking = nullptr;
    Editor* m_editor = nullptr;
    Navigation* m_navigation = nullptr;
    GlobalHotkey* m_globalHotkey = nullptr;
    Database* m_database = nullptr;
    HttpServerManager* m_serverManager = nullptr;
    QString m_findText;

    QMenu* m_eventsMenu = nullptr;
    QAction* m_findNextAction = nullptr;
    QAction* m_findPreviousAction = nullptr;
};
