#pragma once
#include <QSystemTrayIcon>

class MainWindow;

class TrayIcon : public QSystemTrayIcon  {
    Q_OBJECT
public:
    TrayIcon(MainWindow* mainWindow);

private slots:
    void onActivated(QSystemTrayIcon::ActivationReason reason);

private:
    MainWindow* m_mainWindow = nullptr;
    QMenu* m_menu = nullptr;
};
