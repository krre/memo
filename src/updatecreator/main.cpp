#include "MainWindow.h"
#include "Constants.h"
#include "lib/MessageHandler.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {
    qInstallMessageHandler(MemoLib::messageHandler);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(Constants::Organization);
    QCoreApplication::setApplicationName(Constants::ApplicationName);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    MainWindow window;
    window.show();

    return QCoreApplication::exec();
}
