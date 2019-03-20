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
        QString description;
        QString date;
        QVector<QString> os;
        int size;
        QString channel;
    };

    explicit UpdateChecker(QObject* parent = nullptr);

public slots:
    void check();

signals:
    void checkResult(const QVector<Update>& updates);

private:
    void findUpdates(const QJsonObject& manifest);
    const QUrl manifestUrl;
};
