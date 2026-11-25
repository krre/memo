#include "StandardWindow.h"
#include "settings/Settings.h"
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

void StandardWindow::readSettings(Settings* settings) {
    QByteArray geometry = settings->windowGeometry(settingsGroup());

    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        resize(800, 400);
    }
}

void StandardWindow::writeSettings(Settings* settings) {
    settings->setWindowGeometry(settingsGroup(), saveGeometry());
}

Settings* StandardWindow::settings() const {
    return m_settings;
}
