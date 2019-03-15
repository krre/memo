#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkReply>

struct Update {
    QUrl url;
};

class UpdateChecker : public QObject {
    Q_OBJECT
public:
    explicit UpdateChecker(QObject* parent = nullptr);

public slots:
    void check();

signals:
    void checkResult(const Update& update);

private:
    void loadRedirector();
    void loadManifest(const QUrl& manifestUrl);
    void findUpdate(const QJsonObject& manifest);
};
