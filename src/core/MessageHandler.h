#pragma once
#include <QString>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
      QString message = qFormatLogMessage(type, context, msg);

     switch (type) {
         case QtDebugMsg:
             fprintf(stderr, "[Debug] %s\n", qPrintable(message));
             break;
         case QtInfoMsg:
             fprintf(stderr, "[Info] %s\n", qPrintable(message));
             break;
         case QtWarningMsg:
             fprintf(stderr, "[Warning] %s\n", qPrintable(message));
             break;
         case QtCriticalMsg:
             fprintf(stderr, "[Critical] %s\n", qPrintable(message));
             break;
         case QtFatalMsg:
             fprintf(stderr, "[Fatal] %s\n", qPrintable(message));
             break;
    }
}
