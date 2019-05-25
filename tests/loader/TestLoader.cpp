#include <QtTest/QtTest>

class TestLoader : public QObject {
    Q_OBJECT
private slots:
};

QTEST_MAIN(TestLoader)
#include "TestLoader.moc"
