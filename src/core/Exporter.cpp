#include "Exporter.h"
#include "Exception.h"
#include "database/Database.h"
#include "ui/notetaking/NoteTaking.h"
#include "ui/Birthdays.h"
#include "core/Application.h"
#include <QtCore/private/qzipwriter_p.h>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QDirIterator>

void Exporter::exportAll(const QString& filePath, NoteTaking* noteTaking, Database* database, QWidget* parent) {
    QFileInfo fi(filePath);
    QString dirPath = fi.absolutePath() + "/" + fi.baseName();

    auto sg = qScopeGuard([=] {
        QDir dir(dirPath);
        dir.removeRecursively();
    });

    int count = noteTaking->exportNote(0, dirPath + "/notes");
    exportBirthdays(dirPath, database);
    compressDir(dirPath);

    QMessageBox::information(parent, Application::Name, tr("Export Finished. Count of notes: %1").arg(count));
}

void Exporter::exportBirthdays(const QString& dirPath, Database* database) {
    QString filename = dirPath + "/birthdays.txt";
    QFile file(filename);

    if (!file.open(QIODevice::ReadWrite)) return;

    QTextStream stream(&file);

    for (const auto& birthday : database->birthdays()) {
        stream << birthday.date.toString(BirthdayDateFormat) << " " << birthday.name << "\n";
    }
}

void Exporter::compressDir(const QString& dirPath) {
    QZipWriter zipWriter(dirPath + ".zip");

    switch (zipWriter.status()) {
        case QZipWriter::FileWriteError: throw RuntimeError("Write file error");
        case QZipWriter::FileOpenError: throw RuntimeError("Open file error");
        case QZipWriter::FilePermissionsError: throw RuntimeError("Permissions file error");
        case QZipWriter::FileError: throw RuntimeError("File error");
        case QZipWriter::NoError: break;
    }

    zipWriter.setCompressionPolicy(QZipWriter::AutoCompress);
    zipWriter.setCreationPermissions(QFile::permissions(dirPath));

    QFileInfo fi(dirPath);
    zipWriter.addDirectory(fi.fileName());

    QDirIterator it(dirPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString absoluteFilePath = it.next();
        QString relativeFilePath = fi.fileName() + QString(absoluteFilePath).remove(dirPath);

        if (it.fileInfo().isDir()) {
            zipWriter.setCreationPermissions(QFile::permissions(absoluteFilePath));
            zipWriter.addDirectory(relativeFilePath);
        } else if (it.fileInfo().isFile()) {
            QFile file(absoluteFilePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            QByteArray data = file.readAll();

            if (!data.isEmpty()) {
                zipWriter.setCreationPermissions(QFile::permissions(absoluteFilePath));
                zipWriter.addFile(relativeFilePath, data);
            }
        }
    }

    zipWriter.close();
}
