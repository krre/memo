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
#ifdef Q_OS_WIN
    appDir["windows"] = path;
#elif defined (Q_OS_LINUX)
    appDir["linux"] = path;
#elif defined (Q_OS_MACOS)
    appDir["macos"] = path;
#endif

    project["appDir"] = appDir;
}

QString ProjectSettings::appDir() const {
#ifdef Q_OS_WIN
    return project["appDir"].toObject()["windows"].toString();
#elif defined (Q_OS_LINUX)
    return project["appDir"].toObject()["linux"].toString();
#elif defined (Q_OS_MACOS)
    return project["appDir"].toObject()["macos"].toString();
#endif
}

void ProjectSettings::setSnapshot(const QJsonArray& snapshot, const QString& version) {
    QJsonObject snapshots = project["snapshots"].toObject();

    if (!snapshots.contains(version)) {
        snapshots[version] = {};
    }

    QJsonObject os = snapshots[version].toObject();

#ifdef Q_OS_WIN
    os["windows"] = snapshot;
#elif defined (Q_OS_LINUX)
    os["linux"] = snapshot;
#elif defined (Q_OS_MACOS)
    os["macos"] = snapshot;
#endif

    snapshots[version] = os;
    project["snapshots"] = snapshots;
}

QJsonArray ProjectSettings::snapshot(const QString& version) const {
    if (project["snapshots"].toObject().contains(version)) {
        QJsonObject snapshots = project["snapshots"].toObject()[version].toObject();

#ifdef Q_OS_WIN
    return snapshots.contains("windows") ? snapshots["windows"].toArray() : QJsonArray();
#elif defined (Q_OS_LINUX)
    return snapshots.contains("linux") ? snapshots["linux"].toArray() : QJsonArray();
#elif defined (Q_OS_MACOS)
    return snapshots.contains("macos") ? snapshots["macos"].toArray() : QJsonArray();
#endif
    } else {
        return QJsonArray();
    }
}
