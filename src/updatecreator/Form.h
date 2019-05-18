#pragma once
#include "ListModel.h"
#include <QWidget>

class QLineEdit;
class QPlainTextEdit;
class QLabel;
class QComboBox;
class ProjectSettings;

class Form : public QWidget {
    Q_OBJECT
public:
    explicit Form(ProjectSettings* settings, QWidget* parent = nullptr);

    void populateUpdate(const ListModel::Update& update);
    ListModel::Update update() const;

    void setManifestPath(const QString& path);

    void setFileTemplate(const QString& fileTemplate);
    QString fileTemplate() const;

    void setContentDir(const QString& contentDir);
    void setInstallerPath(const QString& installerPath);

    void clear();
    void clearUpdate();

signals:
    void lostFocus();
    void manifestDownloaded();

private slots:
    void onFocusChanged(QWidget* from, QWidget* to);
    void clearSize();
    void selectContentDirectory();
    void selectInstallerFile();
    void build();

    void downloadManifest();
    void uploadManifest();
    void uploadContent();
    void uploadInstaller();


private:
    QWidget* root(QWidget* child);
    void setFileSize(const QString& os, qint64 size);

    ProjectSettings* projectSettings;
    QLabel* manifestLabel = nullptr;
    QLineEdit* templateLineEdit = nullptr;
    QLineEdit* versionLineEdit = nullptr;
    QLineEdit* baseVersionLineEdit = nullptr;
    QLineEdit* dateLineEdit = nullptr;
    QComboBox* channelComboBox = nullptr;
    QLabel* sizeWindowsLabel = nullptr;
    QLabel* sizeLinuxLabel = nullptr;
    QLabel* sizeMacOSLabel = nullptr;
    QPlainTextEdit* descriptionTextEdit = nullptr;
    QLineEdit* contentDirLineEdit = nullptr;
    QLineEdit* installerPathLineEdit = nullptr;
    QString zipPath;
};
