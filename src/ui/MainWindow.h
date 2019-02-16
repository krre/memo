#pragma once
#include <QMainWindow>

class QSplitter;
class QPlainTextEdit;
class Outliner;

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

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();

    QSplitter* splitter;
    Outliner* outliner;
    QPlainTextEdit* textEdit;
};
