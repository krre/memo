#include <QtTest>
#include <database/Database.h>

class Test_UpdateChecker : public QObject {
    Q_OBJECT
private slots:
    void checkDatabase();
};

void Test_UpdateChecker::checkDatabase() {
    Database database;
}

QTEST_MAIN(Test_UpdateChecker)
#include "Test_UpdateChecker.moc"
