#include "Loader.h"
#include <QtCore>

Loader::Loader(const QString srcDir, QString dstDir, QString program, QObject* parent) :
    QObject(parent), srcDir(srcDir), dstDir(dstDir), program(program) {
}

void Loader::start() {
    startTimer(1000);
}

void Loader::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    if (!QFile::exists(srcDir)) {
        qCritical() << "Source not exists:" << srcDir;
        quit();
    }

    if (!QFile::exists(dstDir)) {
        qCritical() << "Destination not exists:" << dstDir;
        quit();
    }

    QDirIterator it(srcDir, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString sourcePath = it.next();
        QDir dir(srcDir);
        QString destinationPath = dstDir + "/" + dir.relativeFilePath(sourcePath);

        qInfo() << QString("Running attempt %1: copy %2 to %3").arg(counter).arg(sourcePath, destinationPath);

        if (!QFile::exists(destinationPath)) {
            QFileInfo fi(destinationPath);

            if (fi.absolutePath() != dstDir) {
                QDir dir;
                dir.mkpath(fi.absolutePath());
            }

            QFile::copy(sourcePath, destinationPath);
            QFile::remove(sourcePath);
        } else if (QFile::remove(destinationPath)) {
            QFile::copy(sourcePath, destinationPath);
            QFile::remove(sourcePath);
        } else {
            counter++;
            // Shutdown process after 10 failed attempts to copy file.
            if (counter > 10) {
                qCritical() << "Failed to copy file:" << sourcePath;
                quit();
            }

            return;
        }
    }

    if (!QFile::exists(program)) {
        qCritical() << "Program not exists:" << program;
        quit();
    }

    qInfo() << "Start program:" << program;
    QProcess::startDetached(program);

    quit();
}

void Loader::quit() {
    QDir dir(srcDir);
    dir.cdUp();
    dir.removeRecursively();

    QCoreApplication::quit();
}
