#include "FileSettings.h"
#include <QSettings>

FileSettings::FileSettings(QObject* parent) : QObject(parent) {

}

void FileSettings::setValue(const QString& key, const QVariant& value) {
    QSettings settings;
    settings.setValue(key, value);
}

QVariant FileSettings::value(const QString& key, const QVariant& defaultValue) const {
    QSettings settings;
    return settings.value(key, defaultValue);
}

void FileSettings::setList(const QString& key, const QVariantList& list) {
    QSettings settings;
    settings.beginWriteArray(key.split("/").first());

    for (int i = 0; i < list.count(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(key.split("/").last(), list.at(i));
    }

    settings.endArray();
}

QVariantList FileSettings::list(const QString& key) const {
    QVariantList result;

    QSettings settings;
    int size = settings.beginReadArray(key.split("/").first());

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        result.append(settings.value(key.split("/").last()));
    }

    settings.endArray();

    return result;
}
