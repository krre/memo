#pragma once
#include <QMainWindow>

class QSplitter;
class QPlainTextEdit;
class QTreeView;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();

    QSplitter* splitter;
    QTreeView* treeView;
    QPlainTextEdit* textEdit;
};
