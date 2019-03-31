#pragma once
#include <QWidget>

class QLineEdit;
class ProjectSettings;

class Builder : public QWidget {
    Q_OBJECT
public:
    explicit Builder(ProjectSettings* settings, QWidget* parent = nullptr);

    void load();
    void clear();

private slots:
    void selectDirectory();

private:
    void createAppDirWidgets();
    void createFilesWidgets();

    QLineEdit* appDirLineEdit = nullptr;
    ProjectSettings* projectSettings;
};
