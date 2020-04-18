#include "ui/MainWindow.h"
#include <core/Constants.h>
#include <core/Context.h>
#include <common/MessageHandler.h>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

void loadLanguage() {
    QSettings settings;

    QString language = settings.value("language").toString();

    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    auto qtTranslator = new QTranslator(qApp);

    if (qtTranslator->load("qt_" + language, QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(qtTranslator);
    }

    auto memoTranslator = new QTranslator(qApp);

    if (memoTranslator->load("memo-" + language, ":/i18n")) {
        QCoreApplication::installTranslator(memoTranslator);
    }
}

int main(int argc, char* argv[]) {
    qInstallMessageHandler(Common::messageHandler);

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(Const::App::Organization);
    QCoreApplication::setApplicationName(Const::App::Name);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("Absent any system tray on this system"));
        return EXIT_FAILURE;
    }

    Context context(&app);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    QApplication::setQuitOnLastWindowClosed(false);
    loadLanguage();
    MainWindow window;

    return QCoreApplication::exec();
}
