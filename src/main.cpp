#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Memo");

    MainWindow window;
    window.show();

    return app.exec();
}
