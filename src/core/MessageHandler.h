#pragma once
#include <QString>

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
     auto localMsg = msg.toLocal8Bit().constData();

     switch (type) {
         case QtDebugMsg:
             fprintf(stderr, "[Debug] %s (%s:%u, %s)\n", localMsg, context.file, context.line, context.function);
             break;
         case QtInfoMsg:
             fprintf(stderr, "[Info] %s\n", localMsg);
             break;
         case QtWarningMsg:
             fprintf(stderr, "[Warning] %s (%s:%u, %s)\n", localMsg, context.file, context.line, context.function);
             break;
         case QtCriticalMsg:
             fprintf(stderr, "[Critical] %s (%s:%u, %s)\n", localMsg, context.file, context.line, context.function);
             break;
         case QtFatalMsg:
             fprintf(stderr, "[Fatal] %s (%s:%u, %s)\n", localMsg, context.file, context.line, context.function);
             break;
    }
}
