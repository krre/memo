#pragma once
#include <QWidget>

class QLineEdit;
class QPlainTextEdit;
class QLabel;
class QComboBox;
class QCheckBox;

class Form : public QWidget {
    Q_OBJECT
public:
    explicit Form(QWidget* parent = nullptr);

private:
    QLineEdit* templateLineEdit = nullptr;
    QLineEdit* versionLineEdit = nullptr;
    QLineEdit* dateLineEdit = nullptr;
    QCheckBox* windowsCheckBox = nullptr;
    QCheckBox* linuxCheckBox = nullptr;
    QCheckBox* macosCheckBox = nullptr;
    QComboBox* channelComboBox = nullptr;
    QLabel* sizeLabel = nullptr;
    QPlainTextEdit* descriptionTextEdit = nullptr;
};
