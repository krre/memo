#pragma once
#include <QMainWindow>

class QSplitter;
class Outliner;
class Form;

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
    void saveFile();
    void quit();
    void about();

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();

    QSplitter* splitter = nullptr;
    Outliner* outliner = nullptr;
    Form* form = nullptr;
};
