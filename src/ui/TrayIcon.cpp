#include "TrayIcon.h"
#include "MainWindow.h"
#include <QtWidgets>

TrayIcon::TrayIcon(MainWindow* mainWindow)
    : QSystemTrayIcon(mainWindow), m_mainWindow(mainWindow) {
    m_menu = new QMenu(mainWindow);
    m_menu->addAction(tr("Show"), m_mainWindow, &QMainWindow::showNormal);
    m_menu->addAction(tr("Hide"), m_mainWindow, &QMainWindow::hide);
    m_menu->addSeparator();
    m_menu->addAction(tr("Exit"), m_mainWindow, &MainWindow::quit);

    connect(this, &QSystemTrayIcon::activated, this, &TrayIcon::onActivated);
    setContextMenu(m_menu);
    setIcon(m_mainWindow->windowIcon());
    show();
}

void TrayIcon::onActivated(ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        m_mainWindow->setVisible(!m_mainWindow->isVisible());
    }
}
