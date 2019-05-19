#include "MainWindow.h"
#include "Editor.h"
#include "Options.h"
#include "core/Constants.h"
#include "core/SqlException.h"
#include "outliner/Outliner.h"
#include "database/Database.h"
#include "hotkey/GlobalHotkey.h"
#include "updater/NewUpdates.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Constants::App::Name);
    setWindowIcon(QIcon(":/images/icon.png"));

    splitter = new QSplitter;
    setCentralWidget(splitter);

    database = new Database(this);

    updateChecker = new UpdateChecker(this);
    connect(updateChecker, &UpdateChecker::checkResult, this, &MainWindow::onCheckUpdatesResult);

    globalHotkey = new GlobalHotkey(this);
    connect(globalHotkey, &GlobalHotkey::activated, this, &MainWindow::showWindow);

    createActions();
    createTrayIcon();
    setupSplitter();

    connect(outliner, &Outliner::noteChanged, this, &MainWindow::onNoteChanged);
    connect(editor, &Editor::focusLost, this, &MainWindow::onEditorFocusLost);
    connect(editor, &Editor::leave, [=] {
       outliner->setFocus();
    });

    readSettings();

    updateMenuState();
}

void MainWindow::readSettings() {
    applyHotSettings();

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

    int size = settings.beginReadArray("RecentFiles");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        addRecentFile(settings.value("path").toString());
    }

    settings.endArray();

    loadFile(settings.value("filePath").toString());

    if (!settings.value("minimizeOnStartup", false).toBool()) {
        show();
    }
}

void MainWindow::writeSettings() {
    QSettings settings;
    settings.setValue("geometry", saveGeometry());
    settings.setValue("splitter", splitter->saveState());
    settings.setValue("filePath", currentFile);

    settings.beginWriteArray("RecentFiles");

    for (int i = 0; i < recentFilesMenu->actions().size() - Constants::Window::SystemRecentFilesActions; ++i) {
        settings.setArrayIndex(i);
        settings.setValue("path", recentFilesMenu->actions().at(i)->text());
    }

    settings.endArray();
}

void MainWindow::applyHotSettings() {
    QSettings settings;
    trayIcon->setVisible(!settings.value("hideTrayIcon").toBool());

    if (settings.value("GlobalHotkey/enabled").toBool()) {
        globalHotkey->setShortcut(settings.value("GlobalHotkey/hotkey", Constants::DefaultSettings::GlobalHotkey).toString());
    } else {
        globalHotkey->unsetShortcut();
    }

    QString fontFamily = settings.value("Editor/fontFamily").toString();

    if (!fontFamily.isEmpty()) {
        QFont font;
        font.setFamily(fontFamily);

        QString fontSize = settings.value("Editor/fontSize").toString();

        if (!fontSize.isEmpty()) {
            font.setPointSize(fontSize.toInt());
        }

        editor->setFont(font);
    }
}

void MainWindow::setupSplitter() {
    outliner = new Outliner(database);
    editor = new Editor;

    splitter->addWidget(outliner);
    splitter->addWidget(editor);

    splitter->setHandleWidth(1);
    splitter->setChildrenCollapsible(false);
    splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), this, &MainWindow::newFile, QKeySequence("Ctrl+N"));
    fileMenu->addAction(tr("Open..."), this, &MainWindow::openFile, QKeySequence("Ctrl+O"));

    recentFilesMenu = new QMenu(tr("Recent Files"), this);
    recentFilesMenu->addSeparator();
    recentFilesMenu->addAction(tr("Clear"), this, &MainWindow::clearMenuRecentFiles);
    fileMenu->addAction(recentFilesMenu->menuAction());

    exportAction = fileMenu->addAction(tr("Export All..."), this, &MainWindow::exportFile, QKeySequence("Ctrl+E"));
    closeAction = fileMenu->addAction(tr("Close"), this, &MainWindow::closeFile, QKeySequence("Ctrl+W"));

    fileMenu->addSeparator();
    fileMenu->addAction(tr("Hide"), this, &MainWindow::hide, QKeySequence("Esc"));
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), this, &MainWindow::quit, QKeySequence("Ctrl+Q"));

    QMenu* toolsMenu = menuBar()->addMenu(tr("Tools"));
    toolsMenu->addAction(tr("Options..."), [this] {
        Options options;

        if (options.exec() == QDialog::Accepted) {
            applyHotSettings();
        }
    });

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("Check for updates..."), updateChecker, &UpdateChecker::check);
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
    trayIcon->setIcon(windowIcon());
    trayIcon->show();
}

void MainWindow::updateMenuState() {
    bool isFileOpen = !currentFile.isEmpty();

    exportAction->setEnabled(isFileOpen);
    closeAction->setEnabled(isFileOpen);
}

void MainWindow::loadFile(const QString& filePath) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) return;

    try {
        database->open(filePath);
        outliner->build();
        setCurrentFile(filePath);
        addRecentFile(filePath);
    } catch (const Memo::Exception& e) {
        showErrorDialog(e.error());
    }
}

void MainWindow::setCurrentFile(const QString& filePath) {
    QString title = QApplication::applicationName();

    if (!filePath.isEmpty()) {
        QFileInfo fi(filePath);
        title = title + " - " + fi.fileName();
    }

    setWindowTitle(title);
    currentFile = filePath;
    updateMenuState();
}

void MainWindow::addRecentFile(const QString& filePath) {
    if (!QFile::exists(filePath)) return;

    for (QAction* action : recentFilesMenu->actions()) {
        if (action->text() == filePath) {
            recentFilesMenu->removeAction(action);
        }
    }

    auto fileAction = new QAction(filePath);
    connect(fileAction, &QAction::triggered, [=] {
        loadFile(filePath);
    });

    recentFilesMenu->insertAction(recentFilesMenu->actions().first(), fileAction);

    if (recentFilesMenu->actions().size() > Constants::Window::MaxRecentFiles + Constants::Window::SystemRecentFilesActions) {
        recentFilesMenu->removeAction(recentFilesMenu->actions().at(recentFilesMenu->actions().size() - Constants::Window::SystemRecentFilesActions - 1));
    }

    updateMenuState();
}

void MainWindow::showErrorDialog(const QString& message) {
    QMessageBox::critical(this, tr("Error"), message, QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::newFile() {
    QString fileName = QFileDialog::getSaveFileName(this, QString(), "notes.db",
                                                    tr("All Files (*);;Database Files (*.db)"));

    if (fileName.isEmpty()) return;

    closeFile();

    if (QFile::exists(fileName)) {
        if (!QFile::remove(fileName)) {
            showErrorDialog(tr("Error rewriting old file"));
        }
    }

    try {
        database->create(fileName);
        setCurrentFile(fileName);
    } catch (const Memo::Exception& e) {
        showErrorDialog(e.error());
    }
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, QString(), QString(),
                                                    tr("All Files (*);;Database Files (*.db)"));
    if (!fileName.isEmpty()) {
        closeFile();
        loadFile(fileName);
    }
}

void MainWindow::exportFile() {
    QString directory = QFileDialog::getExistingDirectory(this);

    if (!directory.isEmpty()) {
        outliner->exportAllNotes(directory);
    }
}

void MainWindow::closeFile() {
    database->close();
    onNoteChanged(0);
    outliner->clear();
    setCurrentFile();
}

void MainWindow::clearMenuRecentFiles() {
    for (int i = recentFilesMenu->actions().size() - Constants::Window::SystemRecentFilesActions - 1; i >= 0; i--) {
        recentFilesMenu->removeAction(recentFilesMenu->actions().at(i));
    }

    updateMenuState();
}

void MainWindow::onCheckUpdatesResult(const QVector<UpdateChecker::Update>& updates) {
    if (updates.isEmpty()) {
        QMessageBox::information(this, tr("Check of updates"), tr("Latest version of %1 is installed").arg(Constants::App::Name));
    } else {
        NewUpdates newUpdates(updates);

        if (newUpdates.exec() == QDialog::Accepted) {
            QString updateDir = newUpdates.getUpdateDir();

            if (!updateDir.isEmpty()) {
                QString loaderPath = qApp->applicationDirPath() + "/loader";
                QProcess::startDetached(loaderPath, QStringList() << updateDir << qApp->applicationDirPath() << qApp->applicationFilePath());
                quit();
            } else {
                qCritical() << "Failed to get update directory";
            }
        }
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About %1").arg(Constants::App::Name),
        tr("<h3>%1 %2 %3</h3>\
           Outliner for quick notes<br><br> \
           Based on Qt %4<br> \
           Build on %5 %6<br><br> \
           <a href=%7>%7</a><br><br>Copyright © %8, Vladimir Zarypov")
            .arg(Constants::App::Name,
                 Constants::App::Version,
                 Constants::App::Status,
                 QT_VERSION_STR,
                 Constants::App::BuildDate,
                 Constants::App::BuildTime,
                 Constants::App::URL,
                 Constants::App::CopyrightYear));
}

void MainWindow::quit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::onNoteChanged(int id) {
    editor->setId(id);
    editor->setEnabled(id > 0);

    if (id) {
        QString note = database->value(id, "note").toString();
        editor->setPlainText(note);
    } else {
        editor->clear();
    }
}

void MainWindow::onEditorFocusLost() {
    int lastId = editor->id();

    if (lastId && editor->document()->isModified()) {
        database->updateValue(lastId, "note", editor->document()->toPlainText());
    }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
    }
}

void MainWindow::showWindow() {
    show();
    raise();
    activateWindow();
}
