#include "RecentFilesMenu.h"
#include "settings/Settings.h"
#include <QFile>

constexpr auto SystemActionCount = 2;
constexpr auto MaxActionCount = 10;

RecentFilesMenu::RecentFilesMenu(Settings* settings, QWidget* parent)
    : QMenu(tr("Recent Files"), parent), m_settings(settings) {
    addSeparator();
    addAction(tr("Clear"), this, &RecentFilesMenu::clear);
    const auto recentFiles = settings->recentFiles();

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

void RecentFilesMenu::save() {
    QStringList recentFiles;

    for (int i = 0; i < actions().size() - SystemActionCount; ++i) {
        recentFiles.append(actions().at(i)->text());
    }

    m_settings->setRecentFiles(recentFiles);
}

void RecentFilesMenu::clear() {
    for (int i = actions().size() - SystemActionCount - 1; i >= 0; --i) {
        removeAction(actions().at(i));
    }
}
