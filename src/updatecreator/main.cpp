#include "MainWindow.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QApplication::setOrganizationName("Memo");
    QApplication::setApplicationName("UpdateCreator");

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    MainWindow window;
    window.show();

    return QApplication::exec();
}
