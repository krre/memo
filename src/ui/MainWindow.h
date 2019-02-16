#pragma once
#include <QMainWindow>
#include <QSystemTrayIcon>

class QSystemTrayIcon;
class QSplitter;
class QPlainTextEdit;
class Outliner;
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
    void about();
    void quit();

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();
    void createTrayIcon();

    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;

    QSplitter* splitter;
    Outliner* outliner;
    QPlainTextEdit* textEdit;

    Database* database;
};
