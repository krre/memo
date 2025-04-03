#include "Navigation.h"

Navigation::Navigation(QObject* parent) : QObject(parent) {

}

void Navigation::go(Id id) {
    if (navigation) {
        navigation = false;
        return;
    }

    if (m_history.count() && m_history.last() == id) {
        return;
    }

    if (m_position >= 0 && m_position < m_history.count() - 1) {
        m_history.erase(m_history.begin() + m_position + 1, m_history.end());
        emit forwardAvailable(false);
    }

    m_history.append(id);
    m_position++;

    if (m_position > 0) {
        emit backAvailable(true);
    }
}

void Navigation::remove(Id id) {
    m_history.removeAll(id);
}

void Navigation::back() {
    if (m_position > 0) {
        navigation = true;
        emit navigate(m_history[--m_position]);
        emit forwardAvailable(true);
    }

    if (m_position == 0) {
        emit backAvailable(false);
    }
}

void Navigation::forward() {
    if (m_position < m_history.count() - 1) {
        navigation = true;
        emit navigate(m_history[++m_position]);
        emit backAvailable(true);
    }

    if (m_position == m_history.count() - 1) {
        emit forwardAvailable(false);
    }
}
