#include <QtTest>

class TestSettings : public QObject {
    Q_OBJECT
public:
    TestSettings();
    ~TestSettings();

private slots:
    void test_case1();

};

TestSettings::TestSettings() {

}

TestSettings::~TestSettings() {

}

void TestSettings::test_case1() {

}

QTEST_MAIN(TestSettings)

#include "tst_testsettings.moc"
