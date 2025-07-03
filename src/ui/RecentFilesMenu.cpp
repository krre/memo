#include "RecentFilesMenu.h"
#include <QFile>

constexpr auto SystemActionCount = 2;
constexpr auto MaxActionCount = 10;

RecentFilesMenu::RecentFilesMenu(const QStringList& recentFiles, QWidget* parent)
    : QMenu(tr("Recent Files"), parent) {
    addSeparator();
    addAction(tr("Clear"), this, &RecentFilesMenu::clear);

    for (const QString& filePath : recentFiles) {
        addPath(filePath);
    }
}

void RecentFilesMenu::addPath(const QString& path) {
    if (path.isEmpty()) return;
    if (!QFile::exists(path)) return;
    const auto pathActions = actions();

    for (QAction* action : pathActions) {
        if (action->text() == path) {
            removeAction(action);
        }
    }

    auto fileAction = new QAction(path);
    connect(fileAction, &QAction::triggered, this, [=, this] {
        emit activated(path);
    });

    insertAction(actions().constFirst(), fileAction);

    if (actions().size() > MaxActionCount + SystemActionCount) {
        removeAction(actions().at(actions().size() - SystemActionCount - 1));
    }
}

QStringList RecentFilesMenu::recentFiles() const {
    QStringList result;

    for (int i = 0; i < actions().size() - SystemActionCount; ++i) {
        result.append(actions().at(i)->text());
    }

    return result;
}

void RecentFilesMenu::clear() {
    for (int i = actions().size() - SystemActionCount - 1; i >= 0; --i) {
        removeAction(actions().at(i));
    }
}
