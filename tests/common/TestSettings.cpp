#include "TestSettings.h"

void TestSettings::setValue(const QString &key, const QVariant &value) {
    settings[key] = value;
}

QVariant TestSettings::value(const QString &key, const QVariant &defaultValue) const {
    return settings.contains(key) ? settings[key] : defaultValue;
}

void TestSettings::setList(const QString &key, const QVariantList &list) {
    settings[key] = list;
}

QVariantList TestSettings::list(const QString &key) const {
    return settings[key].toList();
}
