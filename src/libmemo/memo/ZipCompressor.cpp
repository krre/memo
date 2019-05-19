#include "ZipCompressor.h"
#include "Exception.h"
#include <QtGui/private/qzipwriter_p.h>
#include <QtGui/private/qzipreader_p.h>
#include <QDir>
#include <QDirIterator>
#include <QVector>

namespace Memo {

ZipCompressor::ZipCompressor() {

}

void ZipCompressor::compress(const QString& name, const QString& srcDir) {
    QZipWriter zipWriter(name);

    switch (zipWriter.status()) {
        case QZipWriter::FileWriteError: throw RuntimeError("Write file error");
        case QZipWriter::FileOpenError: throw RuntimeError("Open file error");
        case QZipWriter::FilePermissionsError: throw RuntimeError("Permissions file error");
        case QZipWriter::FileError: throw RuntimeError("File error");
        case QZipWriter::NoError: break;
    }

    zipWriter.setCompressionPolicy(QZipWriter::AutoCompress);
    zipWriter.setCreationPermissions(QFile::permissions(srcDir));

    QFileInfo fi(srcDir);
    QString rootDir = fi.dir().dirName();
    zipWriter.addDirectory(rootDir);

    QDirIterator it(srcDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        QString filePath = it.next();
        if (it.fileInfo().isDir()) {
            zipWriter.setCreationPermissions(QFile::permissions(filePath));
            zipWriter.addDirectory(rootDir + "/" + filePath.remove(srcDir));
        } else if (it.fileInfo().isFile()) {
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            zipWriter.setCreationPermissions(QFile::permissions(filePath));
            zipWriter.addFile(rootDir + "/" + filePath.remove(srcDir), file.readAll());
        }
    }

    zipWriter.close();
}

void ZipCompressor::decompress(const QString& name, const QString& dstDir) {
    QZipReader zipReader(name);

    for (const auto& fileInfo : zipReader.fileInfoList()) {
        if (fileInfo.isDir) {
            QDir dir(dstDir);
            dir.mkpath(dstDir + "/" + fileInfo.filePath);
        } else if (fileInfo.isFile) {
            QString filePath = dstDir + "/" + fileInfo.filePath;
            QFile file(filePath);
            file.open(QFile::WriteOnly);
            file.write(zipReader.fileData(fileInfo.filePath));
            file.setPermissions(fileInfo.permissions);
            file.close();
        }
    }
}

} // Memo
