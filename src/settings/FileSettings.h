#pragma once
#include "Settings.h"

class FileSettings : public Settings {
public:
    FileSettings();

protected:
    void setValue(const QString& key, const QVariant& value) override;
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const override;

    void setList(const QString& key, const QVariantList& list) override;
    QVariantList list(const QString& key) const override;
};
