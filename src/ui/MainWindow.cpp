#include "MainWindow.h"
#include "Editor.h"
#include "Preferences.h"
#include "core/Constants.h"
#include "core/Exception.h"
#include "core/Settings.h"
#include "notetaking/NoteTaking.h"
#include "database/Database.h"
#include "hotkey/GlobalHotkey.h"
#include "server/HttpServer.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Const::App::Name);
    setWindowIcon(QIcon(":/images/icon.png"));

    m_settings = new Settings(this);

    m_splitter = new QSplitter;
    setCentralWidget(m_splitter);

    m_database = new Database(this);
    m_server = new HttpServer(m_database, this);

    m_globalHotkey = new GlobalHotkey(this);
    connect(m_globalHotkey, &GlobalHotkey::activated, this, &MainWindow::onGlobalActivated);

    createActions();
    createTrayIcon();
    setupSplitter();

    connect(m_notetaking, &NoteTaking::noteChanged, this, &MainWindow::onNoteChanged);
    connect(m_editor, &Editor::focusLost, this, &MainWindow::onEditorFocusLost);
    connect(m_editor, &Editor::leave, this, [=] {
       m_notetaking->setFocus();
    });

    readSettings();
    updateMenuState();
}

void MainWindow::readSettings() {
    m_settings->loadAll();

    applyHotSettings();

    if (m_settings->general.geometry.isEmpty()) {
        const QRect availableGeometry = QGuiApplication::screens().constFirst()->availableGeometry();
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(m_settings->general.geometry);
    }

    m_splitter->restoreState(m_settings->general.splitter);

    for (const QString& filePath : m_settings->recentFiles.path) {
        addRecentFile(filePath);
    }

    loadFile(m_settings->general.filePath);

    if (!m_settings->general.minimizeOnStartup) {
        show();
    }
}

void MainWindow::writeSettings() {
    m_settings->general.geometry = saveGeometry();
    m_settings->general.splitter = m_splitter->saveState();
    m_settings->general.filePath = m_currentFile;

    for (int i = 0; i < m_recentFilesMenu->actions().size() - Const::Window::SystemRecentFilesActions; ++i) {
        m_settings->recentFiles.path.append(m_recentFilesMenu->actions().at(i)->text());
    }

    m_settings->saveAll();
}

void MainWindow::applyHotSettings() {
    m_trayIcon->setVisible(!m_settings->general.hideTrayIcon);

    if (m_settings->globalHotKey.enabled) {
        m_globalHotkey->setShortcut(m_settings->globalHotKey.hotKey);
    } else {
        m_globalHotkey->unsetShortcut();
    }

    if (!m_settings->editor.fontFamily.isEmpty()) {
        QFont font;
        font.setFamily(m_settings->editor.fontFamily);

        if (m_settings->editor.fontSize) {
            font.setPointSize(m_settings->editor.fontSize);
        }

        m_editor->setFont(font);
    }

    if (m_settings->server.enabled) {
        if (m_settings->server.key.isEmpty()) {
            qCritical().noquote() << "Server key is empty";
            m_server->stop();
        } else {
            m_server->start(m_settings->server.port, m_settings->server.key);
        }
    } else {
        m_server->stop();
    }
}

void MainWindow::setupSplitter() {
    m_notetaking = new NoteTaking(m_database);
    m_editor = new Editor;

    m_splitter->addWidget(m_notetaking);
    m_splitter->addWidget(m_editor);

    m_splitter->setHandleWidth(1);
    m_splitter->setChildrenCollapsible(false);
    m_splitter->setSizes(QList<int>() << 120 << 500);
}

void MainWindow::createActions() {
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), Qt::CTRL | Qt::Key_N, this, &MainWindow::onNew);
    fileMenu->addAction(tr("Open..."), Qt::CTRL | Qt::Key_O, this, &MainWindow::onOpen);

    m_recentFilesMenu = new QMenu(tr("Recent Files"), this);
    m_recentFilesMenu->addSeparator();
    m_recentFilesMenu->addAction(tr("Clear"), this, &MainWindow::onClearRecentFiles);
    fileMenu->addAction(m_recentFilesMenu->menuAction());

    m_exportAction = fileMenu->addAction(tr("Export All..."), Qt::CTRL | Qt::Key_E, this, &MainWindow::onExport);
    m_exportAction = fileMenu->addAction(tr("Create Backup..."), this, &MainWindow::onBackup);
    m_closeAction = fileMenu->addAction(tr("Close"), Qt::CTRL | Qt::Key_W, this, &MainWindow::onClose);

    fileMenu->addSeparator();
    fileMenu->addAction(tr("Preferences..."), this, &MainWindow::onPreferences);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Hide"), Qt::Key_Escape, this, &MainWindow::hide);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), Qt::CTRL | Qt::Key_Q, this, &MainWindow::onQuit);

    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("Open download page"), [] {
        QDesktopServices::openUrl(QUrl(Const::App::ReleasesUrl));
    });
    helpMenu->addAction(tr("About %1...").arg(Const::App::Name), this, &MainWindow::onAbout);
}

void MainWindow::createTrayIcon() {
    m_trayIconMenu = new QMenu(this);

    m_trayIconMenu->addAction(tr("Show"), this, &QMainWindow::showNormal);
    m_trayIconMenu->addAction(tr("Hide"), this, &QMainWindow::hide);
    m_trayIconMenu->addSeparator();
    m_trayIconMenu->addAction(tr("Exit"), this, &MainWindow::onQuit);

    m_trayIcon = new QSystemTrayIcon(this);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::onTrayIconActivated);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    m_trayIcon->setIcon(windowIcon());
    m_trayIcon->show();
}

void MainWindow::updateMenuState() {
    bool isFileOpen = !m_currentFile.isEmpty();

    m_exportAction->setEnabled(isFileOpen);
    m_closeAction->setEnabled(isFileOpen);
}

void MainWindow::loadFile(const QString& filePath) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) return;

    try {
        m_database->open(filePath);
        m_notetaking->build();
        setCurrentFile(filePath);
        addRecentFile(filePath);
    } catch (const Exception& e) {
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
    m_currentFile = filePath;
    updateMenuState();
}

void MainWindow::addRecentFile(const QString& filePath) {
    if (!QFile::exists(filePath)) return;
    const auto actions = m_recentFilesMenu->actions();

    for (QAction* action : actions) {
        if (action->text() == filePath) {
            m_recentFilesMenu->removeAction(action);
        }
    }

    auto fileAction = new QAction(filePath);
    connect(fileAction, &QAction::triggered, this, [=] {
        loadFile(filePath);
    });

    m_recentFilesMenu->insertAction(m_recentFilesMenu->actions().constFirst(), fileAction);

    if (m_recentFilesMenu->actions().size() > Const::Window::MaxRecentFiles + Const::Window::SystemRecentFilesActions) {
        m_recentFilesMenu->removeAction(m_recentFilesMenu->actions().at(m_recentFilesMenu->actions().size() - Const::Window::SystemRecentFilesActions - 1));
    }

    updateMenuState();
}

void MainWindow::showErrorDialog(const QString& message) {
    QMessageBox::critical(this, tr("Error"), message, QMessageBox::Ok);
}

QString MainWindow::dateFileName(const QString& name) {
    QDateTime dateTime = QDateTime::currentDateTime();
    QFileInfo fi(name);
    return fi.baseName() + "-" + dateTime.date().toString("yyyy-MM-dd") + "_" + dateTime.time().toString("HH-mm-ss") + "." + fi.completeSuffix();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    writeSettings();
    event->accept();
}

void MainWindow::onNew() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), "notes.db",
                                                    tr("All Files (*);;Database Files (*.db)"));

    if (fileName.isEmpty()) return;

    onClose();

    if (QFile::exists(fileName)) {
        if (!QFile::remove(fileName)) {
            showErrorDialog(tr("Error rewriting old file"));
        }
    }

    try {
        m_database->create(fileName);
        loadFile(fileName);
    } catch (const Exception& e) {
        showErrorDialog(e.error());
    }
}

void MainWindow::onOpen() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("All Files (*);;Database Files (*.db)"));
    if (!fileName.isEmpty()) {
        onClose();
        loadFile(fileName);
    }
}

void MainWindow::onExport() {
    QFileInfo fi(m_currentFile);
    QString name = m_settings->backups.directory + "/" + dateFileName(fi.baseName() + ".zip");
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export notes to ZIP archive"), name);

    if (!filePath.isEmpty()) {
        m_notetaking->exportAllNotes(filePath);
    }
}

void MainWindow::onBackup() {
    QFileInfo fi(m_currentFile);
    QString name = m_settings->backups.directory + "/" + dateFileName(fi.fileName());

    QString backupFile = QFileDialog::getSaveFileName(this, tr("Create Backup"), name);

    if (!backupFile.isEmpty()) {
        QFile::copy(m_currentFile, backupFile);
    }
}

void MainWindow::onClose() {
    m_database->close();
    onNoteChanged(0);
    m_notetaking->clear();
    setCurrentFile();
}

void MainWindow::onClearRecentFiles() {
    for (int i = m_recentFilesMenu->actions().size() - Const::Window::SystemRecentFilesActions - 1; i >= 0; i--) {
        m_recentFilesMenu->removeAction(m_recentFilesMenu->actions().at(i));
    }

    updateMenuState();
}

void MainWindow::onPreferences() {
    Preferences preferences(m_settings);

    if (preferences.exec() == QDialog::Accepted) {
        applyHotSettings();
    }
}

void MainWindow::onAbout() {
    using namespace Const::App;

    QMessageBox::about(this, tr("About %1").arg(Name),
        tr("<h3>%1 %2 %3</h3>"
           "Note-taking for quick notes<br><br>"
           "Based on Qt %4<br>"
           "Build on %5 %6<br><br>"
           "<a href=%7>%7</a><br><br>Copyright © %8, Vladimir Zarypov")
            .arg(Name, Version, Status, QT_VERSION_STR, BuildDate, BuildTime, URL, CopyrightYear));
}

void MainWindow::onQuit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::onNoteChanged(Id id) {
    m_editor->setId(id);
    m_editor->setEnabled(id > 0);

    if (id) {
        QString note = m_database->value(id, "note").toString();
        m_editor->setPlainText(note);
        m_editor->setFocus();

        int line = m_database->value(id, "line").toInt();
        QTextCursor cursor = m_editor->textCursor();
        cursor.movePosition(QTextCursor::NextBlock, QTextCursor::MoveAnchor, line);
        m_editor->setTextCursor(cursor);
    } else {
        m_editor->clear();
    }
}

void MainWindow::onEditorFocusLost() {
    Id lastId = m_editor->id();

    if (!lastId) return;

    if (m_editor->document()->isModified()) {
        m_database->updateValue(lastId, "note", m_editor->document()->toPlainText());
    }

    m_database->updateValue(lastId, "line", m_editor->textCursor().blockNumber());
}

void MainWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        setVisible(!isVisible());
    }
}

void MainWindow::onGlobalActivated() {
    show();
    raise();
    activateWindow();
}
