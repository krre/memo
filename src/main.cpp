#include "ui/MainWindow.h"
#include "core/Constants.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName(Constants::App::ORGANIZATIION);
    app.setApplicationName(Constants::App::NAME);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("Absent any system tray on this system"));
        return EXIT_FAILURE;
    }

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QApplication::setQuitOnLastWindowClosed(false);

    MainWindow window;
    window.show();

    return app.exec();
}
