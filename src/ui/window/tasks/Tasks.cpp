#include "Tasks.h"

Tasks::Tasks(Database* database, Settings* settings) : StandardWindow(settings), m_database(database) {
    setWindowTitle(tr("Tasks"));
}

QString Tasks::settingsGroup() const {
    return "Tasks";
}
