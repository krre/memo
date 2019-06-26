#pragma once
#include <QObject>

class QNetworkAccessManager;

class Context : public QObject {
    Q_OBJECT
public:
    Context(QObject* parent = nullptr);

    static QString downloadPageUrl();
    static QString installerUrl(const QString& version);

    static QNetworkAccessManager* networkAccessManager();

private:
    static Context* s_instance;
    QNetworkAccessManager* m_manager = nullptr;
};
