#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkReply>

class UpdateChecker : public QObject {
    Q_OBJECT
public:
    struct Version {
        QString app;
        QString qt;
    };

    struct Update {
        QString url;
        Version version;
        QString description;
        QString date;
        QVector<QString> os;
        int size;
        QString channel;
        bool lite;
        bool isValid = false;
    };

    explicit UpdateChecker(QObject* parent = nullptr);

public slots:
    void check();

signals:
    void checkResult(const Update& latestUpdate, const Update& qtUpdate);

private:
    void loadRedirector();
    void loadManifest(const QUrl& manifestUrl);
    void findUpdate(const QJsonObject& manifest);
    void populateUpdate(const QJsonObject& obj, Update& update);
};
