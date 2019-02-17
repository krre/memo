#include "ui/MainWindow.h"
#include "core/Constants.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName(Constants::App::Organization);
    app.setApplicationName(Constants::App::Name);

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
