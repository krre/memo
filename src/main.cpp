#include "ui/MainWindow.h"
#include "core/Constants.h"
#include "core/App.h"
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char* argv[]) {
    App app(argc, argv);

    App::setOrganizationName(Constants::App::Organization);
    App::setApplicationName(Constants::App::Name);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("Absent any system tray on this system"));
        return EXIT_FAILURE;
    }


#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    QApplication::setQuitOnLastWindowClosed(false);

    QSettings settings;

    QString language = settings.value("language").toString();
    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    QString qtTranslatorFileName = QLatin1String("qt_") + language;
    auto qtTranslator = new QTranslator(&app);

    if (qtTranslator->load(qtTranslatorFileName, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        App::installTranslator(qtTranslator);
    }

    QString memoTranslatorFileName = QLatin1String("memo-") + language;
    auto memoTranslator = new QTranslator(&app);

    if (memoTranslator->load(memoTranslatorFileName, ":/i18n")) {
        App::installTranslator(memoTranslator);
    }

    MainWindow window;

    return App::exec();
}
