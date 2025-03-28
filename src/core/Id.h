#pragma once
#include <QJsonValue>

class Id {
public:
    Id(quint64 id);

    quint64 value() const;
    QJsonValue toJson() const;

    bool operator==(const Id& other) const {
        return m_id == other.m_id;
    }

    bool operator!=(const Id& other) const {
        return !(*this == other);
    }

private:
    quint64 m_id = 0;
};

using Ids = QVector<Id>;
