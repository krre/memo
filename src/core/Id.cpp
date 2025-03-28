#include "Id.h"

Id::Id(quint64 id) : m_id(id) {

}

quint64 Id::value() const {
    return m_id;
}

QJsonValue Id::toJson() const {
    return QJsonValue(static_cast<qint64>(m_id));
}

QString Id::toString() {
    return QString::number(m_id);
}
