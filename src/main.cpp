#include "MainWindow.h"
#include "core/Constants.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(Constants::App::NAME);

    MainWindow window;
    window.show();

    return app.exec();
}
