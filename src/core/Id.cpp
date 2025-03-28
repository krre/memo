#include "Id.h"

Id::Id(quint64 id) : m_id(id) {

}

quint64 Id::value() const {
    return m_id;
}

QJsonValue Id::toJson() const {
    return QJsonValue(static_cast<qint64>(m_id));
}

QString Id::toString() const {
    return QString::number(m_id);
}

QVariant Id::toVariant() const {
    return QVariant(m_id);
}

bool Id::isValid() const {
    return m_id > 0;
}
