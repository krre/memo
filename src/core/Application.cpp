#include "Application.h"
#include "core/Constants.h"
#include "core/MessageHandler.h"
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSystemTrayIcon>

Application::Application() {

}

bool Application::setup() {
    QCoreApplication::setOrganizationName(Const::App::Organization);
    QCoreApplication::setApplicationName(Const::App::Name);

    QApplication::setQuitOnLastWindowClosed(false);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, QObject::tr("Systray"),
                              QObject::tr("Absent any system tray on this system"));
        return false;
    }

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    qInstallMessageHandler(messageHandler);
    loadLanguage();

    return true;
}

void Application::loadLanguage() {
    QSettings settings;

    QString language = settings.value("language").toString();

    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    auto qtTranslator = new QTranslator(qApp);

    if (qtTranslator->load("qt_" + language, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        QCoreApplication::installTranslator(qtTranslator);
    }

    auto memoTranslator = new QTranslator(qApp);

    if (memoTranslator->load("memo-" + language, ":/i18n")) {
        QCoreApplication::installTranslator(memoTranslator);
    }
}
