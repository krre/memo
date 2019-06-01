#include <QtTest>
#include <updatecreator/lib/ListModel.h>

class Test_ListModel : public QObject {
    Q_OBJECT
private slots:
    void addUpdate();
};

void Test_ListModel::addUpdate() {
    ListModel listModel;
}

QTEST_MAIN(Test_ListModel)
#include "Test_ListModel.moc"
