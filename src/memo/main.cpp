#include "ui/MainWindow.h"
#include "core/Constants.h"
#include "core/App.h"
#include <memo/MessageHandler.h>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char* argv[]) {
    qInstallMessageHandler(Memo::messageHandler);

    App app(argc, argv);

    QCoreApplication::setOrganizationName(Constants::App::Organization);
    QCoreApplication::setApplicationName(Constants::App::Name);

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

    auto qtTranslator = new QTranslator(&app);

    if (qtTranslator->load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(qtTranslator);
    }

    auto memoTranslator = new QTranslator(&app);

    if (memoTranslator->load("memo-" + language, ":/i18n")) {
        QCoreApplication::installTranslator(memoTranslator);
    }

    MainWindow window;

    return QCoreApplication::exec();
}
