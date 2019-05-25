#include <QtTest>
#include <core/Context.h>
#include <updater/UpdateChecker.h>

class Test_UpdateChecker : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void check();

private:
    Context* context = nullptr;
};

void Test_UpdateChecker::initTestCase() {
    context = new Context(this);
}

void Test_UpdateChecker::check() {
    UpdateChecker updateChecker;
    qRegisterMetaType<QVector<UpdateChecker::Update>>("QVector<UpdateChecker::Update>");

    QSignalSpy spy(&updateChecker, &UpdateChecker::checkResult);
    updateChecker.check();

    QVERIFY(spy.wait());
    QCOMPARE(spy.count(), 1);

    auto updates = qvariant_cast<QVector<UpdateChecker::Update>>(spy.takeFirst());
    qDebug() << updates.size();
}

QTEST_MAIN(Test_UpdateChecker)
#include "Test_UpdateChecker.moc"
