#include "MainWindow.h"
#include "core/Constants.h"
#include "outliner/Outliner.h"
#include "database/Database.h"
#include <QtCore>
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), splitter(new QSplitter) {
    setWindowTitle(Constants::App::Name);
    setCentralWidget(splitter);

    database = new Database(this);

    createActions();
    createTrayIcon();
    setupSplitter();

    connect(outliner, &Outliner::noteAdded, this, &MainWindow::onNoteAdded);

    readSettings();

    QIcon icon = QIcon(":/images/icon.png");
    setWindowIcon(icon);
    trayIcon->setIcon(icon);
    trayIcon->show();
}

MainWindow::~MainWindow() {
}

void MainWindow::readSettings() {
    QSettings settings;
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();

    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }

    splitter->restoreState(settings.value("splitter").toByteArray());

    loadFile(settings.value("filePath").toString());
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", splitter->saveState());
    settings.setValue("filePath", currentFile);
}

void MainWindow::setupSplitter() {
    outliner = new Outliner;
    textEdit = new QPlainTextEdit;

    splitter->addWidget(outliner);
    splitter->addWidget(textEdit);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), this, &MainWindow::newFile, QKeySequence::New);
    fileMenu->addAction(tr("Open..."), this, &MainWindow::openFile, QKeySequence::Open);
    fileMenu->addAction(tr("Close"), this, &MainWindow::closeFile, QKeySequence::Close);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Hide"), this, &MainWindow::hide, QKeySequence::Cancel);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::quit, QKeySequence::Quit);

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("About %1...").arg(Constants::App::Name), this, &MainWindow::about);
}

void MainWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(tr("Show"), this, &QMainWindow::showNormal);
    trayIconMenu->addAction(tr("Hide"), this, &QMainWindow::hide);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("Exit"), this, &MainWindow::quit);

    trayIcon = new QSystemTrayIcon(this);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::loadFile(const QString filePath) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) return;

    if (database->open(filePath)) {
        currentFile = filePath;
    } else {
        showDatabaseErrorDialog();
    }
}

void MainWindow::showErrorDialog(const QString& message) {
    QMessageBox::critical(this, tr("Error"), message, QMessageBox::Ok);
}

void MainWindow::showDatabaseErrorDialog() {
    showErrorDialog("Database error");
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::newFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), "notes.db",
                                tr("All Files (*);;Database Files (*.db)"), &selectedFilter);

    if (fileName.isEmpty()) return;

    if (QFile::exists(fileName)) {
        closeFile();

        if (!QFile::remove(fileName)) {
            showErrorDialog(tr("Error rewriting old file"));
        }
    }

    if (!database->create(fileName)) {
        showDatabaseErrorDialog();
    }

    currentFile = fileName;
}

void MainWindow::openFile() {
    QString selectedFilter;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                tr("All Files (*);;Database Files (*.db)"), &selectedFilter);
    if (!fileName.isEmpty() && !database->open(fileName)) {
        showDatabaseErrorDialog();
    }

    currentFile = fileName;
}

void MainWindow::closeFile() {
    database->close();
    currentFile = QString();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About %1").arg(Constants::App::Name),
        tr("<h3>%1 %2 %3</h3>\
           Outliner for quick notes<br><br> \
           Based on Qt %4<br> \
           Build on %5<br><br> \
           <a href=%6>%6</a><br><br>%7")
            .arg(Constants::App::Name)
            .arg(Constants::App::Version).arg(Constants::App::Status)
            .arg(QT_VERSION_STR)
            .arg(__DATE__)
            .arg(Constants::App::URL).arg(Constants::App::Copyright));
}

void MainWindow::quit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::onNoteAdded(int parent, int pos, const QString& title) {
    int id = database->insertRecord(parent, pos, title);
    qDebug() << id;
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
    }
}
