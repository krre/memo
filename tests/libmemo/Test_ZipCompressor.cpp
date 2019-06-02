#include <QtTest>
#include <common/ZipCompressor.h>

class Test_ZipCompressor : public QObject {
    Q_OBJECT
private slots:
    void compressDecompress();
};

void Test_ZipCompressor::compressDecompress() {
    // Compress test
    QTemporaryDir tmpDir1;

    QDir dir(tmpDir1.path());
    dir.mkdir("test");

    QString srcPath = tmpDir1.path() + "/test/";
    QString name = tmpDir1.path() + "/test.zip";

    {
        QFile file1(srcPath + "/file1.txt");
        file1.open(QFile::WriteOnly);
        file1.write("file1");
        file1.close();

        QDir dir1(srcPath);
        dir1.mkdir("dir1");

        QString dir1Path = srcPath + "/dir1";

        QFile file2(dir1Path + "/file2.txt");
        file2.open(QFile::WriteOnly);
        file2.write("file2");
        file2.close();
    }

    Common::ZipCompressor::compress(name, srcPath);

    QVERIFY(QFile::exists(name));

    // Decompress test
    QTemporaryDir tmpDir2;

    QString dstDir = tmpDir2.path();

    Common::ZipCompressor::decompress(name, dstDir);

    QString testDirPath = dstDir + "/test";
    QVERIFY(QDir(testDirPath).exists());

    {
        QFile file1(testDirPath + "/file1.txt");
        QVERIFY(file1.exists());

        file1.open(QFile::ReadOnly);
        QVERIFY(file1.readAll() == "file1");

        QString dir1Path = testDirPath + "/dir1";
        QVERIFY(QDir(dir1Path).exists());

        QFile file2(dir1Path + "/file2.txt");
        QVERIFY(file2.exists());

        file2.open(QFile::ReadOnly);
        QVERIFY(file2.readAll() == "file2");
    }
}

QTEST_MAIN(Test_ZipCompressor)
#include "Test_ZipCompressor.moc"
