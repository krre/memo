#pragma once
#include <QObject>

class Loader : public QObject {
    Q_OBJECT
public:
    Loader(const QString srcDir, QString dstDir, QString program, QObject* parent = nullptr);
    void start();

protected:
    void timerEvent(QTimerEvent* event);

private:
    void quit();

    QString srcDir;
    QString dstDir;
    QString program;

    int counter = 1;
    int logNum = 0;
};
