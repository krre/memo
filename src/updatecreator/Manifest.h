#pragma once
#include <QWidget>
#include "ListModel.h"

class ProjectSettings;
class QLineEdit;
class QPlainTextEdit;
class QLabel;
class QComboBox;

class Manifest : public QWidget {
    Q_OBJECT
public:
    explicit Manifest(ProjectSettings* settings, QWidget* parent = nullptr);

    void populateUpdate(const ListModel::Update& update);
    ListModel::Update getUpdate() const;

    void setManifestPath(const QString& path);

    void setFileTemplate(const QString& fileTemplate);
    QString getFileTemplate() const;

    void setFileSize(const QString& os, qint64 size);
    QString getVersion();

    void clear();
    void clearUpdate();

signals:
    void lostFocus();

private slots:
    void onFocusChanged(QWidget* from, QWidget* to);
    void upload();

private:
    ProjectSettings* projectSettings;
    QWidget* root(QWidget* child);

    QLabel* manifestLabel = nullptr;
    QLineEdit* templateLineEdit = nullptr;
    QLineEdit* versionLineEdit = nullptr;
    QLineEdit* dateLineEdit = nullptr;
    QComboBox* channelComboBox = nullptr;
    QLabel* sizeWindowsLabel = nullptr;
    QLabel* sizeLinuxLabel = nullptr;
    QLabel* sizeMacOSLabel = nullptr;
    QPlainTextEdit* descriptionTextEdit = nullptr;
};
