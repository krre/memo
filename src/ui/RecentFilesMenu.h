#pragma once
#include <QMenu>

class RecentFilesMenu : public QMenu {
    Q_OBJECT
public:
    RecentFilesMenu(const QStringList& recentFiles, QWidget* parent = nullptr);

    void addPath(const QString& path);
    QStringList recentFiles() const;

signals:
    void activated(const QString& path);

private slots:
    void clear();
};
