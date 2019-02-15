#include "MainWindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(800, 500);
    setWindowTitle(qApp->applicationName());
}

MainWindow::~MainWindow() {
}
