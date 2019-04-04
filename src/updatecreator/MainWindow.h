#pragma once
#include <QMainWindow>

class QSplitter;
class QTabWidget;
class Outliner;
class Manifest;
class Builder;
class ListModel;
class ProjectSettings;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void newProject();
    void openProject();
    void saveProject();
    void closeProject();

    void clearMenuRecentProjects();

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
    void setProjectPath(const QString& path);
    void addRecentProject(const QString& path);
    void loadProject(const QString& path);

    QSplitter* splitter = nullptr;
    QTabWidget* tabWidget = nullptr;
    ProjectSettings* projectSettings = nullptr;
    Outliner* outliner = nullptr;
    Manifest* manifest = nullptr;
    Builder* builder = nullptr;
    ListModel* listModel = nullptr;
    QString manifestPath;
    QString projectPath;

    QAction* saveAction = nullptr;
    QAction* closeAction = nullptr;
    QMenu* recentProjectsMenu = nullptr;
};
