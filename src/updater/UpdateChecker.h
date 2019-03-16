#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkReply>

class UpdateChecker : public QObject {
    Q_OBJECT
public:
    struct Update {
        QUrl url;
        bool isValid;
    };

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
