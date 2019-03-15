#include "ui/MainWindow.h"
#include "core/Constants.h"
#include "core/App.h"
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char* argv[]) {
    App app(argc, argv);

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
    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    QString qtTranslatorFileName = QLatin1String("qt_") + language;
    QTranslator* qtTranslator = new QTranslator(&app);

    if (qtTranslator->load(qtTranslatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        app.installTranslator(qtTranslator);
    }

    QString memoTranslatorFileName = QLatin1String("memo-") + language;
    QTranslator* memoTranslator = new QTranslator(&app);

    if (memoTranslator->load(memoTranslatorFileName, ":/i18n")) {
        app.installTranslator(memoTranslator);
    }

    MainWindow window;

    return app.exec();
}
