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
    void newProject();
    void openProject();
    void saveProject();
    void closeProject();

    void quit();
    void about();

    void addUpdate();
    void removeUpdate(int row);

private:
    void readSettings();
    void writeSettings();

    void saveManifest();
    void openManifest();
    void closeManifest();

    void createActions();
    void updateActions();

    void setupSplitter();
    bool wantSave();
    void setProjectPath(const QString& path);

    void markDirty();
    void clearDirty();

    QSplitter* splitter = nullptr;
    QTabWidget* tabWidget = nullptr;
    Outliner* outliner = nullptr;
    Form* form = nullptr;
    Builder* builder = nullptr;
    ListModel* listModel = nullptr;
    bool dirty = false;
    QString manifestPath;
    QString projectPath;

    QAction* saveAction = nullptr;
    QAction* closeAction = nullptr;
};
