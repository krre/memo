#include "EditorToolBar.h"
#include "Navigation.h"

EditorToolBar::EditorToolBar(Navigation* navigation) {
    auto backAction = addAction(QIcon(":/assets/icons/arrow-left.svg"), QString(), navigation, &Navigation::back);
    backAction->setEnabled(false);
    connect(navigation, &Navigation::backAvailable, backAction, &QAction::setEnabled);

    auto forwardAction = addAction(QIcon(":/assets/icons/arrow-right.svg"), QString(), navigation, &Navigation::forward);
    forwardAction->setEnabled(false);
    connect(navigation, &Navigation::forwardAvailable, forwardAction, &QAction::setEnabled);

    auto clearAction = addAction(QIcon(":/assets/icons/trash.svg"), QString(), navigation, &Navigation::clear);
    clearAction->setEnabled(false);
    connect(navigation, &Navigation::clearAvailable, clearAction, &QAction::setEnabled);
}
