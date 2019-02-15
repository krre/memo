#include "ui/MainWindow.h"
#include "core/Constants.h"
#include <QApplication>
#include <QSettings>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setOrganizationName(Constants::App::ORGANIZATIION);
    app.setApplicationName(Constants::App::NAME);

    QSettings::setDefaultFormat(QSettings::IniFormat);

    MainWindow window;
    window.show();

    return app.exec();
}
