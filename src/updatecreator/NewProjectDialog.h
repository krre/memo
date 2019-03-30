#pragma once
#include <QDialog>

class QLineEdit;

class NewProjectDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewProjectDialog(QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void selectAppDir();
    void selectWorkspaceDir();

private:
    QLineEdit* appLineEdit = nullptr;
    QLineEdit* workspaceLineEdit = nullptr;
};
