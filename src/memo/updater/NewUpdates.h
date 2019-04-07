#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class QProgressBar;
class UpdateDownloader;

class NewUpdates : public QDialog {
    Q_OBJECT

public:
    NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent = nullptr);

public slots:
    void reject() override;

private slots:
    void startUpdate();
    void finishUpdate(const QString& updateDir);

private:
    QString sizeToMegabyte(int size);

    UpdateDownloader* updateDownloader = nullptr;
    QPushButton* updateButton = nullptr;
    QProgressBar* progressBar = nullptr;
    QVector<QUrl> urls;
};
