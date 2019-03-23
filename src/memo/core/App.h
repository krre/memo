#pragma once
#include <QApplication>

class QNetworkAccessManager;

class App : public QApplication {
    Q_OBJECT

public:
    App(int& argc, char** argv);

    static QNetworkAccessManager* networkAccessManager();

private:
    static App* instance;

    QNetworkAccessManager* nam;
};
