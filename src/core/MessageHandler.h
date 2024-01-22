#pragma once
#include <QString>

void messageHandler(QtMsgType type, const QMessageLogContext& context [[maybe_unused]], const QString& msg) {
     auto localMsg = msg.toLocal8Bit().constData();

     switch (type) {
         case QtDebugMsg:
             fprintf(stderr, "[Debug] %s\n", localMsg);
             break;
         case QtInfoMsg:
             fprintf(stderr, "[Info] %s\n", localMsg);
             break;
         case QtWarningMsg:
             fprintf(stderr, "[Warning] %s\n", localMsg);
             break;
         case QtCriticalMsg:
             fprintf(stderr, "[Critical] %s\n", localMsg);
             break;
         case QtFatalMsg:
             fprintf(stderr, "[Fatal] %s\n", localMsg);
             break;
    }
}
