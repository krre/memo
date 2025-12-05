#pragma once
#include <QToolBar>

class Navigation;
class QLabel;

class EditorToolBar : public QToolBar {
    Q_OBJECT
public:
    EditorToolBar(Navigation* navigation);

    void setSymbolsCount(int count);

private:
    QLabel* m_symbolsLabel = nullptr;
};
