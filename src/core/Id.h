#pragma once
#include <QVector>

class Id {
public:
    Id(quint64 id);

    quint64 value() const;

private:
    quint64 m_id = 0;
};

using Ids = QVector<Id>;
