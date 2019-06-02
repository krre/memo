#include <QtTest>
#include <loader/lib/Loader.h>

class Test_Loader : public QObject {
    Q_OBJECT
private slots:
    void loaderRun();
};

void Test_Loader::loaderRun() {
    QString srcDir;
    QString dstDir;
    QString program;
    Loader loader(srcDir, dstDir, program);
}

QTEST_MAIN(Test_Loader)
#include "Test_Loader.moc"
