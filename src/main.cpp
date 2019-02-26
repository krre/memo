#include "ui/MainWindow.h"
#include "core/Constants.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>

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

    QSettings settings;
    QString language = settings.value("language").toString();

    QTranslator translator;

    if(language.isEmpty()) {
        translator.load(QLocale(), QLatin1String("memo"), QLatin1String("-"), QLatin1String(":/i18n"));
    } else {
        translator.load(QString("memo-%1").arg(language), ":/i18n");
    }

    app.installTranslator(&translator);

    MainWindow window;
    window.show();

    return app.exec();
}
