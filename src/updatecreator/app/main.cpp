#include "MainWindow.h"
#include "Constants.h"
#include <common/MessageHandler.h>
#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {
    qInstallMessageHandler(Common::messageHandler);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(Const::Organization);
    QCoreApplication::setApplicationName(Const::ApplicationName);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    MainWindow window;
    window.show();

    return QCoreApplication::exec();
}
