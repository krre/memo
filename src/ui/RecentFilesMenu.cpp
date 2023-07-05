#include "RecentFilesMenu.h"
#include "core/Settings.h"
#include <QFile>

constexpr auto systemActionCount = 2;
constexpr auto maxActionCount = 10;


RecentFilesMenu::RecentFilesMenu(const QString& title, QWidget* parent) : QMenu(title, parent) {
    addSeparator();
    addAction(tr("Clear"), this, &RecentFilesMenu::clear);

    for (const QString& filePath : Settings::value<SettingsKey::RecentFiles::Name>()) {
        addPath(filePath);
    }
}

RecentFilesMenu::~RecentFilesMenu() {
    QStringList pathes;

    for (int i = 0; i < actions().size() - systemActionCount; ++i) {
        pathes.append(actions().at(i)->text());
    }

    Settings::setValue<SettingsKey::RecentFiles::Name>(pathes);
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

void RecentFilesMenu::clear() {
    for (int i = actions().size() - systemActionCount - 1; i >= 0; i--) {
        removeAction(actions().at(i));
    }
}
