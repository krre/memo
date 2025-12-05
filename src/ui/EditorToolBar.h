#pragma once
#include <QToolBar>

class Navigation;

class QLabel;
class QAction;

class EditorToolBar : public QToolBar {
    Q_OBJECT
public:
    EditorToolBar(Navigation* navigation);

    void setSymbolsCount(int count);
    void setSymbolsCountVisible(bool visible);

private:
    QLabel* m_symbolsCountLabel = nullptr;
    QAction* m_symbolsCountAction = nullptr;
};
