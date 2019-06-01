#include <QtTest>
#include <loader/lib/Loader.h>

class Test_Loader : public QObject {
    Q_OBJECT
private slots:
    void loaderRun();
};

void Test_Loader::loaderRun() {
    Loader loader("", "", "");
}

QTEST_MAIN(Test_Loader)
#include "Test_Loader.moc"
