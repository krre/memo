#include "App.h"
#include "Constants.h"
#include <memo/Constants.h>
#include <QtNetwork>

App* App::instance = nullptr;

App::App(int& argc, char** argv) : QApplication(argc, argv) {
    instance = this;
    nam = new QNetworkAccessManager(this);
}

QNetworkAccessManager* App::networkAccessManager() {
    return instance->nam;
}

QString App::downloadPageUrl() {
    return QString(Constants::App::DownloadUrl) + "/install/" + Memo::Constants::CurrentOS;
}

QString App::installerUrl(const QString& version) {
#if defined Q_OS_LINUX
    QString installerName = QString("%1-%2.tar.gz").arg(qApp->applicationName(), version);
#elif defined Q_OS_WIN
    QString installerName = QString("%1-%2.exe").arg(qApp->applicationName(), version);
#elif defined Q_OS_MACOS
    QString installerName = QString("%1-%2.dmg").arg(qApp->applicationName(), version);
#endif

    return downloadPageUrl() + "/" + installerName;
}
