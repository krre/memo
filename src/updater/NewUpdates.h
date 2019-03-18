#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class QProgressBar;

class NewUpdates : public QDialog {
    Q_OBJECT

public:
    NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent = nullptr);

private slots:
    void startUpdate();

private:
    QString sizeToMegabyte(int size);

    QPushButton* updateButton;
    QProgressBar* progressBar;
    QVector<QUrl> urls;
    int bytesCounter = 0;
};
