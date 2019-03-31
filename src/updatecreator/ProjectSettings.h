#pragma once
#include <QObject>

class ProjectSettings : public QObject {
    Q_OBJECT
public:
    explicit ProjectSettings(QObject* parent = nullptr);

    void create(const QString& path);
    void open(const QString& path);
    void save();
    void close();

private:
    QString filePath;
};
