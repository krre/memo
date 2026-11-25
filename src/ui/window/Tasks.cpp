#include "Tasks.h"
#include "settings/Settings.h"

Tasks::Tasks(Database* database, Settings* settings) : StandardWindow(settings), m_database(database) {
    setWindowTitle(tr("Tasks"));
}

void Tasks::readSettings(Settings* settings) {
    QByteArray geometry = settings->tasksGeometry();

    if (!geometry.isEmpty()) {
        restoreGeometry(geometry);
    } else {
        resize(800, 400);
    }
}

void Tasks::writeSettings(Settings* settings) {
    settings->setTasksGeometry(saveGeometry());
}
