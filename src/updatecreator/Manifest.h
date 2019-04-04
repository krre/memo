#pragma once
#include <QWidget>
#include "ListModel.h"

class QLineEdit;
class QPlainTextEdit;
class QLabel;
class QComboBox;
class QCheckBox;

class Manifest : public QWidget {
    Q_OBJECT
public:
    explicit Manifest(QWidget* parent = nullptr);

    void populateUpdate(const ListModel::Update& update);
    ListModel::Update getUpdate() const;

    void setManifestPath(const QString path);

    void setUrl(const QString& url);
    QString getUrl() const;

    void clear();

signals:
    void lostFocus();

private slots:
    void onFocusChanged(QWidget* from, QWidget* to);

private:
    QWidget* root(QWidget* child);

    QLabel* manifestLabel = nullptr;
    QLineEdit* urlLineEdit = nullptr;
    QLineEdit* versionLineEdit = nullptr;
    QLineEdit* dateLineEdit = nullptr;
    QCheckBox* windowsCheckBox = nullptr;
    QCheckBox* linuxCheckBox = nullptr;
    QCheckBox* macosCheckBox = nullptr;
    QComboBox* channelComboBox = nullptr;
    QLabel* sizeLabel = nullptr;
    QPlainTextEdit* descriptionTextEdit = nullptr;

    int fileSize = 0;
};
