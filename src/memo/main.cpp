#include "ui/MainWindow.h"
#include "core/Constants.h"
#include "core/App.h"
#include <QMessageBox>
#include <QSettings>
#include <QTranslator>
#include <QLibraryInfo>

void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
     QByteArray localMsg = msg.toLocal8Bit();
     switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "[Debug] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         break;
     case QtInfoMsg:
         fprintf(stderr, "[Info] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         break;
     case QtWarningMsg:
         fprintf(stderr, "[Warning] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "[Critical] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         break;
     case QtFatalMsg:
         fprintf(stderr, "[Fatal] %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
         break;
    }
}

int main(int argc, char* argv[]) {
    qInstallMessageHandler(messageOutput);

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
