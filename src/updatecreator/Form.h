#pragma once
#include <QWidget>
#include "ListModel.h"

class QLineEdit;
class QPlainTextEdit;
class QLabel;
class QComboBox;
class QCheckBox;

class Form : public QWidget {
    Q_OBJECT
public:
    explicit Form(QWidget* parent = nullptr);

    void populateUpdate(const ListModel::Update& update);
    ListModel::Update getUpdate() const;

    void setUrl(const QString& url);
    QString getUrl() const;

private:
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
