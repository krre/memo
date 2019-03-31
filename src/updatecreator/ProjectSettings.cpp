#include "ProjectSettings.h"
#include "Constants.h"
#include "lib/Exception.h"
#include <QtCore>

ProjectSettings::ProjectSettings(QObject* parent) : QObject(parent) {

}

void ProjectSettings::create(const QString& path) {
    filePath = path;

    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        throw MemoLib::RuntimeError(tr("Failed to open project file for writting"));
    }

    QJsonObject appDir = { { "windows", "" }, { "linux", "" }, { "macos", "" } };

    QJsonObject project;
    project["appDir"] = appDir;

    file.write(QJsonDocument(project).toJson());
}

void ProjectSettings::open(const QString& path) {
    filePath = path;
    qDebug() << "open project settings" << path;
}

void ProjectSettings::save() {
    if (filePath.isEmpty()) return;

    qDebug() << "save project settings" << filePath;
}

void ProjectSettings::close() {
    if (filePath.isEmpty()) return;

    save();
    qDebug() << "close project settings" << filePath;
    filePath = QString();
}
