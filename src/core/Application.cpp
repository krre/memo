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
