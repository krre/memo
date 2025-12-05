#include "EditorToolBar.h"
#include "Navigation.h"
#include <QLabel>

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

    auto spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addWidget(spacer);

    m_symbolsCountLabel = new QLabel;
    m_symbolsCountAction = addWidget(m_symbolsCountLabel);
}

void EditorToolBar::setSymbolsCount(int count) {
    m_symbolsCountLabel->setText(tr("Symbols: %1").arg(count));
}

void EditorToolBar::setSymbolsCountVisible(bool visible) {
    m_symbolsCountAction->setVisible(visible);
}
