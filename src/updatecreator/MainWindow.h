#pragma once
#include <QMainWindow>

class QSplitter;

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
    void quit();
    void about();

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();

    QSplitter* splitter = nullptr;
};
