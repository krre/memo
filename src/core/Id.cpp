#include "Id.h"

Id::Id(quint64 id) : m_id(id) {

}

quint64 Id::value() const {
    return m_id;
}
