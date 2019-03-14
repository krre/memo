#pragma once
#include <QObject>
#include <QUrl>

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
};
