#include "ProjectSettings.h"
#include <common/Exception.h>
#include <common/Constants.h>
#include <QtCore>

ProjectSettings::ProjectSettings(QObject* parent) : QObject(parent) {

}

void ProjectSettings::create(const QString& path) {
    filePath = path;

    project["contentDir"] = {};
    save();
}

void ProjectSettings::open(const QString& path) {
    filePath = path;

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        throw Common::RuntimeError(tr("Failed to open project file for reading"));
    }

    project = QJsonDocument::fromJson(file.readAll()).object();
}

void ProjectSettings::save() {
    if (filePath.isEmpty()) return;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        throw Common::RuntimeError(tr("Failed to open project file for writting"));
    }

    file.write(QJsonDocument(project).toJson());
}

void ProjectSettings::close() {
    if (filePath.isEmpty()) return;

    save();
    filePath = QString();
    project = QJsonObject();
}

void ProjectSettings::setContentDir(const QString& path) {
    QJsonObject contentDir = project["contentDir"].toObject();
    contentDir[Common::Constants::CurrentOS] = path;
    project["contentDir"] = contentDir;
}

QString ProjectSettings::contentDir() const {
    return project["contentDir"].toObject()[Common::Constants::CurrentOS].toString();
}

void ProjectSettings::setInstallerPath(const QString& path) {
    QJsonObject installerPath = project["installerPath"].toObject();
    installerPath[Common::Constants::CurrentOS] = path;
    project["installerPath"] = installerPath;
}

QString ProjectSettings::installerPath() const {
    return project["installerPath"].toObject()[Common::Constants::CurrentOS].toString();
}

QString ProjectSettings::projectDir() const {
    QFileInfo fi(filePath);
    return fi.absolutePath();
}

void ProjectSettings::setFtpData(const ProjectSettings::FtpData& data) {
    QJsonObject ftp;
    ftp["updateUrl"] = data.updateUrl;
    ftp["installUrl"] = data.installUrl;
    ftp["login"] = data.login;
    ftp["password"] = data.password;

    project["ftp"] = ftp;
    save();
}

ProjectSettings::FtpData ProjectSettings::ftpData() const {
    FtpData data;

    if (project.contains("ftp")) {
        QJsonObject ftp = project["ftp"].toObject();
        data.updateUrl = ftp["updateUrl"].toString();
        data.installUrl = ftp["installUrl"].toString();
        data.login = ftp["login"].toString();
        data.password = ftp["password"].toString();
    }

    return data;
}
