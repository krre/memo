#pragma once
#include <QToolBar>

class Navigation;

class EditorToolBar : public QToolBar {
public:
    EditorToolBar(Navigation* navigation);
};
