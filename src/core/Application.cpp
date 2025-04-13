#include "Application.h"
#include "core/MessageHandler.h"
#include "settings/FileSettings.h"
#include <QTranslator>
#include <QLibraryInfo>
#include <QSettings>

Application::Application(int& argc, char* argv[]) : QApplication(argc, argv) {
    setOrganizationName(Application::Organization);
    setApplicationName(Application::Name);
    setQuitOnLastWindowClosed(false);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    qInstallMessageHandler(messageHandler);
    installTranslators();
}

void Application::installTranslators() {
    FileSettings settings;
    QString language = settings.applicationLanguage();

    if (language.isEmpty()) {
        language = QLocale::system().name().split("_").first();
    }

    auto qtTranslator = new QTranslator(this);

    if (qtTranslator->load("qt_" + language, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        installTranslator(qtTranslator);
    }

    auto qtbaseTranslator = new QTranslator(this);

    if (qtbaseTranslator->load("qtbase_" + language, QLibraryInfo::path(QLibraryInfo::TranslationsPath))) {
        installTranslator(qtbaseTranslator);
    }

    auto appTranslator = new QTranslator(this);

    if (appTranslator->load(QString("%1_%2").arg(PROJECT_NAME, language), ":/i18n")) {
        installTranslator(appTranslator);
    }
}
