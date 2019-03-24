#pragma once
#include <QMainWindow>

class QSplitter;
class Outliner;
class Form;
class ListModel;

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
    bool saveFile();
    void quit();
    void about();

    void addUpdate();
    void removeUpdate(int row);

private:
    void readSettings();
    void writeSettings();

    void setupSplitter();
    void createActions();
    bool wantQuit();
    void saveManifest();

    QSplitter* splitter = nullptr;
    Outliner* outliner = nullptr;
    Form* form = nullptr;
    ListModel* listModel = nullptr;
    bool dirty = false;
    QString filePath;
};
