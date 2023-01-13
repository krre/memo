#include <core/Application.h>
#include <ui/MainWindow.h>
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    if (!Application::setup()) {
        return EXIT_FAILURE;
    }

    MainWindow window;

    return QCoreApplication::exec();
}
