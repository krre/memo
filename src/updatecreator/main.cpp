#include "MainWindow.h"
#include "Constants.h"
#include "lib/MessageHandler.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {
    qInstallMessageHandler(MemoLib::messageHandler);

    QApplication app(argc, argv);

    QApplication::setOrganizationName(Constants::Organization);
    QApplication::setApplicationName(Constants::ApplicationName);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    MainWindow window;
    window.show();

    return QApplication::exec();
}
