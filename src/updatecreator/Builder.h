#pragma once
#include <QWidget>

class QLineEdit;
class QListWidget;
class ProjectSettings;
class Manifest;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(ProjectSettings* settings, Manifest* manifest, QWidget* parent = nullptr);

    void load();
    void clear();

    void createSnapshot(const QString& version);
    void selectSnapshot(const QString& version);
    void removeSnapshot(const QString& version);

    void setVersion(const QString& version);

private slots:
    void selectDirectory();
    void refresh();
    void build();
    void upload();

private:
    void createAppDirWidgets();
    void createFilesWidgets();

    QByteArray fileChecksum(const QString& filePath);

    ProjectSettings* projectSettings;
    Manifest* manifest;

    QLineEdit* appDirLineEdit = nullptr;
    QListWidget* allFilesListWidget = nullptr;
    QListWidget* modifiedFilesListWidget = nullptr;
    QString version;
    QString zipPath;
};
