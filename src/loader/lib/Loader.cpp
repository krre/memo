#include "Loader.h"
#include <QtCore>

Loader::Loader(const QString& srcDir, QString& dstDir, QString& program, QObject* parent) :
    QObject(parent), m_srcDir(srcDir), m_dstDir(dstDir), m_program(program) {
}

void Loader::start() {
    startTimer(1000);
}

void Loader::timerEvent(QTimerEvent* event) {
    Q_UNUSED(event)

    if (!QFile::exists(m_srcDir)) {
        qCritical() << "Source not exists:" << m_srcDir;
        quit();
    }

    if (!QFile::exists(m_dstDir)) {
        qCritical() << "Destination not exists:" << m_dstDir;
        quit();
    }

    QDirIterator it(m_srcDir, QStringList() << "*", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString sourcePath = it.next();
        QDir dir(m_srcDir);
        QString destinationPath = m_dstDir + "/" + dir.relativeFilePath(sourcePath);

        qInfo() << QString("Running attempt %1: copy %2 to %3").arg(m_counter).arg(sourcePath, destinationPath);

        if (!QFile::exists(destinationPath)) {
            QFileInfo fi(destinationPath);

            if (fi.absolutePath() != m_dstDir) {
                QDir dir;
                dir.mkpath(fi.absolutePath());
            }

            QFile::copy(sourcePath, destinationPath);
            QFile::remove(sourcePath);
        } else if (QFile::remove(destinationPath)) {
            QFile::copy(sourcePath, destinationPath);
            QFile::remove(sourcePath);
        } else {
            m_counter++;
            // Shutdown process after 10 failed attempts to copy file.
            if (m_counter > 10) {
                qCritical() << "Failed to copy file:" << sourcePath;
                quit();
            }

            return;
        }
    }

    if (!QFile::exists(m_program)) {
        qCritical() << "Program not exists:" << m_program;
        quit();
    }

    // Fix bug with unseting x-flag after decompressing zip on non-Windows OS.
    QFile::setPermissions(m_program, QFile::permissions(m_program) | QFile::ExeOwner);

    qInfo() << "Start program:" << m_program;
    QProcess::startDetached(m_program);

    quit();
}

void Loader::quit() {
    QDir dir(m_srcDir);
    dir.cdUp();
    dir.removeRecursively();

    QCoreApplication::quit();
}
