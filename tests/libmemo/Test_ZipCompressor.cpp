#include <QtTest>
#include <memo/ZipCompressor.h>

class Test_ZipCompressor : public QObject {
    Q_OBJECT
private slots:
    void compressorRun();
};

void Test_ZipCompressor::compressorRun() {
    Memo::ZipCompressor zipCompressor;
}

QTEST_MAIN(Test_ZipCompressor)
#include "Test_ZipCompressor.moc"
