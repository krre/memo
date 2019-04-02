#pragma once
#include <QObject>
#include <QJsonObject>

class ProjectSettings : public QObject {
    Q_OBJECT
public:
    explicit ProjectSettings(QObject* parent = nullptr);

    void create(const QString& path);
    void open(const QString& path);
    void save();
    void close();

    void setAppDir(const QString& path);
    QString appDir() const;

    void setSnapshot(const QJsonArray& snapshot, const QString& version);

private:
    QString filePath;
    QJsonObject project;
};
