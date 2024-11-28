#include "Settings.h"

void Settings::setApplicationLanguage(const QString& lang) {
    setValue("Application/language", lang);
}

QString Settings::applicationLanguage() const {
    return value("Application/language").toString();
}

void Settings::setApplicationFilePath(const QString& filePath) {
    setValue("Application/filePath", filePath);
}

QString Settings::applicationFilePath() const {
    return value("Application/filePath").toString();
}

void Settings::setApplicationMinimizeOnStartup(bool minimizeOnStartup) {
    setValue("Application/minimizeOnStartup", minimizeOnStartup);
}

bool Settings::applicationMinimizeOnStartup() const {
    return value("Application/minimizeOnStartup").toBool();
}

void Settings::setApplicationHideTrayIcon(bool hideTrayIcon) {
    setValue("Application/hideTrayIcon", hideTrayIcon);
}

bool Settings::applicationHideTrayIcon() const {
    return value("Application/hideTrayIcon").toBool();
}
