#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>
#include "updater/UpdateChecker.h"

class QSplitter;
class Outliner;
class Editor;
class Database;
class GlobalHotkey;
class UpdateChecker;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override = default;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void openFile();
    void exportFile();
    void closeFile();
    void clearMenuRecentFiles();
    void about();
    void quit();

    void onNoteChanged(int id);
    void onEditorFocusLost();
    void onCheckUpdatesResult(const QVector<UpdateChecker::Update>& updates);

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void showWindow();

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
    void showDatabaseErrorDialog();

    QSystemTrayIcon* trayIcon = nullptr;
    QMenu* trayIconMenu = nullptr;
    QMenu* recentFilesMenu = nullptr;
    QAction* exportAction = nullptr;
    QAction* closeAction = nullptr;

    QSplitter* splitter = nullptr;
    Outliner* outliner = nullptr;
    Editor* editor = nullptr;
    GlobalHotkey* globalHotkey = nullptr;
    UpdateChecker* updateChecker = nullptr;

    Database* database = nullptr;
    QString currentFile;
};
