#pragma once
#include <QObject>
#include <QJsonObject>

class ProjectSettings : public QObject {
    Q_OBJECT
public:
    struct FtpData {
        QString updateUrl;
        QString installUrl;
        QString login;
        QString password;
    };

    explicit ProjectSettings(QObject* parent = nullptr);

    void create(const QString& path);
    void open(const QString& path);
    void save();
    void close();

    void setContentDir(const QString& path);
    QString contentDir() const;

    void setInstallerPath(const QString& path);
    QString installerPath() const;

    QString projectDir() const;

    void setFtpData(const FtpData& data);
    FtpData ftpData() const;

private:
    QString filePath;
    QJsonObject project;
};
