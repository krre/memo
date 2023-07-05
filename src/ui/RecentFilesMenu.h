#pragma once
#include <QMenu>

class RecentFilesMenu : public QMenu {
    Q_OBJECT
public:
    RecentFilesMenu(const QString& title, QWidget* parent = nullptr);
    ~RecentFilesMenu();

    void addPath(const QString& path);

signals:
    void activated(const QString& path);

private slots:
    void clear();
};
