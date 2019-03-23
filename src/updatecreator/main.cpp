#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("Memo");
    QApplication::setApplicationName("UpdateCreator");

    MainWindow window;
    window.show();

    return QApplication::exec();
}
