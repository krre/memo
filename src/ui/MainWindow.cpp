#include "MainWindow.h"
#include "core/Constants.h"
#include <QApplication>
#include <QSettings>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    resize(800, 500);
    setWindowTitle(Constants::App::NAME);
}

MainWindow::~MainWindow() {
}
