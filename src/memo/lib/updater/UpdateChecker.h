#pragma once
#include <QObject>
#include <QUrl>
#include <QNetworkReply>

class UpdateChecker : public QObject {
    Q_OBJECT
public:
    struct Update {
        QUrl url;
        QString version;
        QString baseVersion;
        QString description;
        QString date;
        int size = 0;
        QString channel;
    };

    explicit UpdateChecker(QObject* parent = nullptr);

public slots:
    void check();

signals:
    void checkResult(const QVector<UpdateChecker::Update>& updates);

private:
    void findUpdates(const QJsonObject& manifest);
    bool isChannelSuit(const QString& current, const QString& mapOn);

    const QUrl m_manifestUrl;
};

Q_DECLARE_METATYPE(UpdateChecker::Update)
