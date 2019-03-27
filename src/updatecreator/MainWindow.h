#pragma once
#include <QMainWindow>

class QSplitter;
class QTabWidget;
class Outliner;
class Form;
class Builder;
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
    void closeFile();
    void quit();
    void about();

    void addUpdate();
    void removeUpdate(int row);

private:
    void readSettings();
    void writeSettings();

    void newManifest();
    void saveManifest();
    void openManifest(const QString& filePath);
    void closeManifest();

    void createActions();
    void updateActions();

    void setupSplitter();
    bool wantSave();
    void changeWindowTitle();

    void markDirty();
    void clearDirty();

    QSplitter* splitter = nullptr;
    QTabWidget* tabWidget = nullptr;
    Outliner* outliner = nullptr;
    Form* form = nullptr;
    Builder* builder = nullptr;
    ListModel* listModel = nullptr;
    bool dirty = false;
    QString filePath;

    QAction* saveAction = nullptr;
    QAction* closeAction = nullptr;
};
