#include <core/Application.h>
#include <ui/MainWindow.h>
#include <QApplication>

int main(int argc, char* argv[]) {
    // Hack to fix styling with Qt 6.5 on GTK3
    qunsetenv("GNOME_DESKTOP_SESSION_ID");
    qunsetenv("XDG_CURRENT_DESKTOP");

    QApplication app(argc, argv);

    if (!Application::setup()) {
        return EXIT_FAILURE;
    }

    MainWindow window;

    return QCoreApplication::exec();
}
