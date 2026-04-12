#pragma once
#include <QToolBar>

class Navigation;
class Editor;

class QLabel;
class QAction;

class EditorToolBar : public QToolBar {
    Q_OBJECT
public:
    EditorToolBar(Navigation* navigation, Editor* editor);

    void setSymbolsCount(int count);
    void setSymbolsCountVisible(bool visible);

signals:
    void saveClicked();

private:
    QLabel* m_symbolsCountLabel = nullptr;
    QAction* m_symbolsCountAction = nullptr;
};
