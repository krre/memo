#include <core/Application.h>
#include <ui/MainWindow.h>
#include <QMessageBox>
#include <QSystemTrayIcon>

int main(int argc, char* argv[]) {
#ifdef Q_OS_LINUX
    // Hack to fix styling with Qt 6.5 on GTK3
    qunsetenv("GNOME_DESKTOP_SESSION_ID");
    qunsetenv("XDG_CURRENT_DESKTOP");
#endif

    Application app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, Application::Name, QObject::tr("Absent any system tray on this system"));
        return EXIT_FAILURE;
    }

    MainWindow window;

    return app.exec();
}
