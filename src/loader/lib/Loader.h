#pragma once
#include <QObject>

class Loader : public QObject {
    Q_OBJECT
public:
    Loader(const QString& srcDir, QString& dstDir, QString& program, QObject* parent = nullptr);
    void start();

protected:
    void timerEvent(QTimerEvent* event) override;

private:
    void quit();

    QString m_srcDir;
    QString m_dstDir;
    QString m_program;

    int m_counter = 1;
    int m_logNum = 0;
};
