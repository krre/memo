#include "ProjectSettings.h"
#include "Constants.h"
#include "lib/Exception.h"
#include <QtCore>

ProjectSettings::ProjectSettings(QObject* parent) : QObject(parent) {

}

void ProjectSettings::create(const QString& path) {
    filePath = path;

    project["appDir"] = {};
    project["snapshots"] = {};

    save();
}

void ProjectSettings::open(const QString& path) {
    filePath = path;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        throw MemoLib::RuntimeError(tr("Failed to open project file for reading"));
    }

    project = QJsonDocument::fromJson(file.readAll()).object();
}

void ProjectSettings::save() {
    if (filePath.isEmpty()) return;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        throw MemoLib::RuntimeError(tr("Failed to open project file for writting"));
    }

    file.write(QJsonDocument(project).toJson());
}

void ProjectSettings::close() {
    if (filePath.isEmpty()) return;

    save();
    filePath = QString();
    project = QJsonObject();
}

void ProjectSettings::setAppDir(const QString& path) {
    QJsonObject appDir = project["appDir"].toObject();
    appDir[Constants::CurrentOS] = path;
    project["appDir"] = appDir;
}

QString ProjectSettings::appDir() const {
    return project["appDir"].toObject()[Constants::CurrentOS].toString();
}

QString ProjectSettings::projectDir() const {
    QFileInfo fi(filePath);
    return fi.absolutePath();
}

void ProjectSettings::setFtpData(const ProjectSettings::FtpData& data) {
    QJsonObject ftp;
    ftp["url"] = data.url;
    ftp["login"] = data.login;
    ftp["password"] = data.password;

    project["ftp"] = ftp;
    save();
}

ProjectSettings::FtpData ProjectSettings::ftpData() const {
    FtpData data;

    if (project.contains("ftp")) {
        QJsonObject ftp = project["ftp"].toObject();
        data.url = ftp["url"].toString();
        data.login = ftp["login"].toString();
        data.password = ftp["password"].toString();
    }

    return data;
}

void ProjectSettings::setSnapshot(const QJsonArray& snapshot, const QString& version) {
    QJsonObject snapshots = project["snapshots"].toObject();

    if (!snapshots.contains(version)) {
        snapshots[version] = {};
    }

    QJsonObject os = snapshots[version].toObject();
    os[Constants::CurrentOS] = snapshot;
    snapshots[version] = os;
    project["snapshots"] = snapshots;
    save();
}

QJsonArray ProjectSettings::snapshot(const QString& version) const {
    QJsonObject snapshots = project["snapshots"].toObject()[version].toObject();
    return snapshots.contains(Constants::CurrentOS) ? snapshots[Constants::CurrentOS].toArray() : QJsonArray();
}

void ProjectSettings::removeSnapshot(const QString& version) {
    QJsonObject snapshots = project["snapshots"].toObject();
    snapshots.remove(version);
    project["snapshots"] = snapshots;
    save();
}

void ProjectSettings::replaceVersion(const QString& oldVersion, const QString& newVersion) {
    if (oldVersion == newVersion) return;

    QJsonObject snapshots = project["snapshots"].toObject();
    QJsonObject snapshotAllOS = snapshots[oldVersion].toObject();
    snapshots[newVersion] = snapshotAllOS;
    snapshots.remove(oldVersion);
    project["snapshots"] = snapshots;
    save();
}
