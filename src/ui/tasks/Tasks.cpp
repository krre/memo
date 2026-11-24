#include "Tasks.h"
#include "settings/Settings.h"
#include <QCloseEvent>

Tasks::Tasks(Database* database, Settings* settings) : m_database(database), m_settings(settings) {
    setWindowTitle(tr("Tasks"));
    setAttribute(Qt::WA_DeleteOnClose, true);
    readSettings();
}

void Tasks::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void Tasks::readSettings() {
    QByteArray geometry = m_settings->tasksGeometry();

    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        resize(800, 400);
    }
}

void Tasks::writeSettings() {
    m_settings->setTasksGeometry(saveGeometry());
}
