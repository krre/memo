#include "App.h"
#include <QtNetwork>

App* App::instance = nullptr;

App::App(int& argc, char** argv) : QApplication(argc, argv) {
    instance = this;
    nam = new QNetworkAccessManager(this);
}

QNetworkAccessManager*App::networkAccessManager() {
    return instance->nam;
}
