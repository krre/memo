#include "RecentFilesMenu.h"
#include "settings/Settings.h"
#include <QFile>

constexpr auto systemActionCount = 2;
constexpr auto maxActionCount = 10;

RecentFilesMenu::RecentFilesMenu(Settings* settings, QWidget* parent)
    : QMenu(tr("Recent Files"), parent), m_settings(settings) {
    addSeparator();
    addAction(tr("Clear"), this, &RecentFilesMenu::clear);

    for (const QString& filePath : settings->recent().files) {
        addPath(filePath);
    }
}

void RecentFilesMenu::addPath(const QString& path) {
    if (path.isEmpty()) return;
    if (!QFile::exists(path)) return;

    for (QAction* action : actions()) {
        if (action->text() == path) {
            removeAction(action);
        }
    }

    auto fileAction = new QAction(path);
    connect(fileAction, &QAction::triggered, this, [=, this] {
        emit activated(path);
    });

    insertAction(actions().constFirst(), fileAction);

    if (actions().size() > maxActionCount + systemActionCount) {
        removeAction(actions().at(actions().size() - systemActionCount - 1));
    }
}

void RecentFilesMenu::save() {
    Settings::Recent recent;

    for (int i = 0; i < actions().size() - systemActionCount; ++i) {
        recent.files.append(actions().at(i)->text());
    }

    m_settings->setRecent(recent);
}

void RecentFilesMenu::clear() {
    for (int i = actions().size() - systemActionCount - 1; i >= 0; i--) {
        removeAction(actions().at(i));
    }
}
