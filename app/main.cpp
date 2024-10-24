#include <core/Application.h>
#include <ui/MainWindow.h>
#include <QMessageBox>
#include <QSystemTrayIcon>

int main(int argc, char* argv[]) {
    Application app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, Application::Name, QObject::tr("Absent any system tray on this system"));
        return EXIT_FAILURE;
    }

    MainWindow window;

    return app.exec();
}
