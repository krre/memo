#pragma once
#include <QDialog>

class QLineEdit;

class NewProjectDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewProjectDialog(QWidget* parent = nullptr);
    QString projectDir() const;

public slots:
    void accept() override;

private slots:
    void selectDirectory();

private:
    void createProject();

    QLineEdit* nameLineEdit = nullptr;
    QLineEdit* directoryLineEdit = nullptr;
    QString projectDirPath;
};
