#pragma once
#include <QDialog>

class QCheckBox;

class ClearSizeDialog : public QDialog {
    Q_OBJECT
public:
    explicit ClearSizeDialog(QWidget* parent = nullptr);

    const QVector<QString> selectedOS() const { return os; }

public slots:
    void accept() override;

private:
    QCheckBox* windowsCheckBox = nullptr;
    QCheckBox* linuxCheckBox = nullptr;
    QCheckBox* macOSCheckBox = nullptr;

    QVector<QString> os;
};
