#pragma once
#include <QWidget>

class QLineEdit;
class QListWidget;
class ProjectSettings;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(ProjectSettings* settings, QWidget* parent = nullptr);

    void load();
    void clear();

    void createSnapshot(const QString& version = "base");
    void removeSnapshot(const QString& version);

private slots:
    void selectDirectory();

private:
    void createAppDirWidgets();
    void createFilesWidgets();

    QByteArray fileChecksum(const QString& filePath);

    QLineEdit* appDirLineEdit = nullptr;
    ProjectSettings* projectSettings;
    QListWidget* allFilesListWidget = nullptr;
    QListWidget* modifiedFilesListWidget = nullptr;
};
