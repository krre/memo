#include "StandardWindow.h"
#include <QCloseEvent>

StandardWindow::StandardWindow(Settings* settings) : m_settings(settings) {
    setAttribute(Qt::WA_DeleteOnClose, true);
}

void StandardWindow::showEvent(QShowEvent* event) {
    if (!m_settingsLoaded) {
        readSettings(m_settings);
        m_settingsLoaded = true;
    }
}

void StandardWindow::closeEvent(QCloseEvent* event) {
    writeSettings(m_settings);
    event->accept();
}

Settings* StandardWindow::settings() const {
    return m_settings;
}
