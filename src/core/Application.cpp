#include "Application.h"
#include "core/Constants.h"
#include "core/MessageHandler.h"
#include "settings/FileSettings.h"
#include <QMessageBox>
#include <QTranslator>
#include <QLibraryInfo>
#include <QSystemTrayIcon>
#include <QSettings>

Application::Application(int& argc, char* argv[]) : QApplication(argc, argv) {
    setOrganizationName(Const::App::Organization);
    setApplicationName(Const::App::Name);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    qInstallMessageHandler(messageHandler);
    installTranslators();
}

bool Application::setup() {
    setQuitOnLastWindowClosed(false);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(nullptr, Const::App::Name, QObject::tr("Absent any system tray on this system"));
        return false;
    }

    return true;
}

void Application::installTranslators() {
    FileSettings settings;
    QString language = settings.general().language;

    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    auto qtTranslator = new QTranslator(qApp);

    if (qtTranslator->load("qt_" + language, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        installTranslator(qtTranslator);
    }

    auto memoTranslator = new QTranslator(qApp);

    if (memoTranslator->load("memo-" + language, ":/i18n")) {
        installTranslator(memoTranslator);
    }
}
