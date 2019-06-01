#pragma once
#include "UpdateChecker.h"
#include <QDialog>

class QProgressBar;
class UpdateDownloader;

class NewUpdates : public QDialog {
    Q_OBJECT
public:
    NewUpdates(const QVector<UpdateChecker::Update>& updates, QWidget* parent = nullptr);
    QString updateDir() const;

public slots:
    void reject() override;

private slots:
    void startUpdate();
    void finishUpdate(const QString& updateDir);

private:
    QString sizeToMegabyte(int size);

    UpdateDownloader* m_updateDownloader = nullptr;
    QPushButton* m_updateButton = nullptr;
    QProgressBar* m_progressBar = nullptr;
    QUrl m_url;
    QString m_updateDir;
};
