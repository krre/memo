#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>

class QSystemTrayIcon;
class QSplitter;
class Outliner;
class Editor;
class Database;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newFile();
    void openFile();
    void closeFile();
    void clearMenuRecentFiles();
    void about();
    void quit();

    void onNoteChanged(int id);

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();
    void createTrayIcon();
    void updateMenuState();

    void loadFile(const QString filePath);
    void setCurrentFile(const QString& filePath = QString());
    void addRecentFile(const QString filePath);

    void showErrorDialog(const QString& message);
    void showDatabaseErrorDialog();

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;
    QMenu* recentFilesMenu;
    QAction* closeAction;

    QSplitter* splitter;
    Outliner* outliner;
    Editor* editor;

    Database* database;
    QString currentFile;
    int currentId = 0;
};
