#pragma once
#include <QMenu>

class Settings;

class RecentFilesMenu : public QMenu {
    Q_OBJECT
public:
    RecentFilesMenu(Settings* settings, QWidget* parent = nullptr);

    void addPath(const QString& path);
    void save();

signals:
    void activated(const QString& path);

private slots:
    void clear();

private:
    Settings* m_settings = nullptr;
};
