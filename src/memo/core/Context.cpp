#include "Context.h"
#include "Constants.h"
#include <memo/Constants.h>
#include <QtNetwork>

Context* Context::instance = nullptr;

Context::Context(QObject* parent) : QObject (parent) {
    Q_ASSERT(!instance);
    instance = this;
    nam = new QNetworkAccessManager(this);
}

QNetworkAccessManager* Context::networkAccessManager() {
    return instance->nam;
}

QString Context::downloadPageUrl() {
    return QString(Constants::App::DownloadUrl) + "/install/" + Memo::Constants::CurrentOS;
}

QString Context::installerUrl(const QString& version) {
#if defined Q_OS_LINUX
    QString installerName = QString("%1-%2.tar.gz").arg(qApp->applicationName(), version);
#elif defined Q_OS_WIN
    QString installerName = QString("%1-%2.zip").arg(qApp->applicationName(), version);
#elif defined Q_OS_MACOS
    QString installerName = QString("%1-%2.dmg").arg(qApp->applicationName(), version);
#endif

    return downloadPageUrl() + "/" + installerName;
}
