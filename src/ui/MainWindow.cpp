#include "MainWindow.h"
#include "RecentFilesMenu.h"
#include "Editor.h"
#include "TrayIcon.h"
#include "FindText.h"
#include "Preferences.h"
#include "Birthdays.h"
#include "core/Constants.h"
#include "core/Exception.h"
#include "core/Settings.h"
#include "core/SolidString.h"
#include "notetaking/NoteTaking.h"
#include "database/Database.h"
#include "hotkey/GlobalHotkey.h"
#include "server/HttpServerManager.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle(Const::App::Name);
    setWindowIcon(QIcon(":/images/icon.png"));

    m_splitter = new QSplitter;
    setCentralWidget(m_splitter);

    m_database = new Database(this);
    m_serverManager = new HttpServerManager(m_database, this);

    m_globalHotkey = new GlobalHotkey(this);
    connect(m_globalHotkey, &GlobalHotkey::activated, this, &MainWindow::onGlobalActivated);

    m_trayIcon = new TrayIcon(this);

    setupSplitter();
    createActions();

    connect(m_notetaking, &NoteTaking::noteChanged, this, &MainWindow::onNoteChanged);
    connect(m_editor, &Editor::focusLost, this, &MainWindow::onEditorFocusLost);
    connect(m_editor, &Editor::leave, this, [this] {
       m_notetaking->setFocus();
    });

    readSettings();
}

void MainWindow::quit() {
    writeSettings();
    QCoreApplication::quit();
}

void MainWindow::readSettings() {
    using namespace SettingsKey;

    applyHotSettings();

    if (Settings::contains<General::Geometry>()) {
        restoreGeometry(Settings::value<General::Geometry>());
    } else {
        const QRect availableGeometry = QGuiApplication::screens().constFirst()->availableGeometry();
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2, (availableGeometry.height() - height()) / 2);
    }

    m_splitter->restoreState(Settings::value<General::Splitter>());

    loadFile(Settings::value<General::FilePath>());

    if (!Settings::value<General::MinimizeOnStartup>()) {
        show();
    }
}

void MainWindow::writeSettings() {
    using namespace SettingsKey;

    Settings::setValue<General::Geometry>(saveGeometry());
    Settings::setValue<General::Splitter>(m_splitter->saveState());
    Settings::setValue<General::FilePath>(m_currentFile);
}

void MainWindow::applyHotSettings() {
    using namespace SettingsKey;

    m_trayIcon->setVisible(!Settings::value<General::HideTrayIcon>());

    if (Settings::value<SettingsKey::GlobalHotkey::Enabled>()) {
        m_globalHotkey->setShortcut(Settings::value<SettingsKey::GlobalHotkey::Hotkey>());
    } else {
        m_globalHotkey->unsetShortcut();
    }

    if (!Settings::value<SettingsKey::Editor::FontFamily>().isEmpty()) {
        QFont font;
        font.setFamily(Settings::value<SettingsKey::Editor::FontFamily>());

        if (Settings::value<SettingsKey::Editor::FontSize>()) {
            font.setPointSize(Settings::value<SettingsKey::Editor::FontSize>());
        }

        m_editor->setFont(font);
    }

    m_serverManager->stop();

    if (!Settings::value<Server::Enabled>()) {
        return;
    }

    QString token = Settings::value<Server::Token>();

    if (token.isEmpty()) {
        qCritical().noquote() << "Server token is empty";
        return;
    }

    QString certificate = Settings::value<Server::Certificate>();

    if (certificate.isEmpty()) {
        qCritical().noquote() << "Server SSL certificate path is empty";
        return;
    }

    QString privateKey = Settings::value<Server::PrivateKey>();

    if (privateKey.isEmpty()) {
        qCritical().noquote() << "Server SSL private key is empty";
        return;
    }

    int port = Settings::value<Server::Port>();

    m_serverManager->start(port, SolidString(token), SolidString(certificate), SolidString(privateKey));
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
    auto fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(tr("New..."), Qt::CTRL | Qt::Key_N, this, &MainWindow::createFile);
    fileMenu->addAction(tr("Open..."), Qt::CTRL | Qt::Key_O, this, &MainWindow::open);

    m_recentFilesMenu = new RecentFilesMenu(tr("Recent Files"), this);
    connect(m_recentFilesMenu, &RecentFilesMenu::activated, this, &MainWindow::loadFile);
    fileMenu->addAction(m_recentFilesMenu->menuAction());

    auto exportAction = fileMenu->addAction(tr("Export All..."), Qt::CTRL | Qt::Key_E, this, &MainWindow::exportAll);
    auto createBackupAction = fileMenu->addAction(tr("Create Backup..."), this, &MainWindow::backup);
    auto closeAction = fileMenu->addAction(tr("Close"), Qt::CTRL | Qt::Key_W, this, &MainWindow::closeFile);

    exportAction->setEnabled(false);
    createBackupAction->setEnabled(false);
    closeAction->setEnabled(false);

    connect(this, &MainWindow::isOpened, exportAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, createBackupAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, closeAction, &QAction::setEnabled);

    fileMenu->addSeparator();
    fileMenu->addAction(tr("Preferences..."), this, &MainWindow::showPreferences);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Hide"), Qt::Key_Escape, this, &MainWindow::hide);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Exit"), Qt::CTRL | Qt::Key_Q, this, &MainWindow::quit);

    auto editMenu = menuBar()->addMenu(tr("Edit"));
    auto undoAction = editMenu->addAction(tr("Undo"), QKeySequence::Undo, m_editor, &Editor::undo);
    auto redoAction = editMenu->addAction(tr("Redo"), QKeySequence::Redo, m_editor, &Editor::redo);
    editMenu->addSeparator();
    auto cutAction = editMenu->addAction(tr("Cut"), QKeySequence::Cut, m_editor, &Editor::cut);
    auto copyAction = editMenu->addAction(tr("Copy"), QKeySequence::Copy, m_editor, &Editor::copy);
    auto pasteAction = editMenu->addAction(tr("Paste"), QKeySequence::Paste, m_editor, &Editor::paste);
    editMenu->addSeparator();
    auto selectAllAction = editMenu->addAction(tr("Select All"), QKeySequence::SelectAll, m_editor, &Editor::selectAll);
    editMenu->addSeparator();
    auto findAction = editMenu->addAction(tr("Find..."), QKeySequence::Find, this, &MainWindow::find);
    m_findNextAction = editMenu->addAction(tr("Find Next"), QKeySequence::FindNext, this, &MainWindow::findNext);
    m_findPreviousAction = editMenu->addAction(tr("Find Previous"), QKeySequence::FindPrevious, this, &MainWindow::findPrevious);

    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    cutAction->setEnabled(false);
    copyAction->setEnabled(false);
    selectAllAction->setEnabled(false);
    findAction->setEnabled(false);
    m_findNextAction->setEnabled(false);
    m_findPreviousAction->setEnabled(false);

    connect(m_editor, &QPlainTextEdit::undoAvailable, undoAction, &QAction::setEnabled);
    connect(m_editor, &QPlainTextEdit::redoAvailable, redoAction, &QAction::setEnabled);
    connect(m_editor, &QPlainTextEdit::copyAvailable, cutAction, &QAction::setEnabled);
    connect(m_editor, &QPlainTextEdit::copyAvailable, copyAction, &QAction::setEnabled);
    connect(m_editor, &QPlainTextEdit::textChanged, this, [=, this] {
        selectAllAction->setEnabled(!m_editor->document()->isEmpty());
    });
    connect(this, &MainWindow::isOpened, pasteAction, &QAction::setEnabled);
    connect(this, &MainWindow::isOpened, findAction, &QAction::setEnabled);

    auto eventsMenu = menuBar()->addMenu(tr("Events"));
    eventsMenu->addAction(tr("Birthdays..."), this, &MainWindow::showBirthdays);

    auto helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(tr("Open download page"), [] {
        QDesktopServices::openUrl(QUrl(Const::App::ReleasesUrl));
    });
    helpMenu->addAction(tr("About %1...").arg(Const::App::Name), this, &MainWindow::about);
}

void MainWindow::loadFile(const QString& filePath) {
    if (filePath.isEmpty() || !QFile::exists(filePath)) return;

    try {
        m_database->open(filePath);
        m_notetaking->build();
        setCurrentFile(filePath);
        m_recentFilesMenu->addPath(filePath);
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
    emit isOpened(!filePath.isEmpty());
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

void MainWindow::createFile() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), "notes.db",
                                                    tr("All Files (*);;Database Files (*.db)"));

    if (fileName.isEmpty()) return;

    closeFile();

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

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("All Files (*);;Database Files (*.db)"));
    if (!fileName.isEmpty()) {
        closeFile();
        loadFile(fileName);
    }
}

void MainWindow::exportAll() {
    QFileInfo fi(m_currentFile);
    QString name = Settings::value<SettingsKey::Backups::Directory>() + "/" + dateFileName(fi.baseName() + ".zip");
    QString filePath = QFileDialog::getSaveFileName(this, tr("Export notes to ZIP archive"), name);

    if (!filePath.isEmpty()) {
        m_notetaking->exportAllNotes(filePath);
    }
}

void MainWindow::backup() {
    QFileInfo fi(m_currentFile);
    QString name = Settings::value<SettingsKey::Backups::Directory>() + "/" + dateFileName(fi.fileName());

    QString backupFile = QFileDialog::getSaveFileName(this, tr("Create Backup"), name);

    if (!backupFile.isEmpty()) {
        QFile::copy(m_currentFile, backupFile);
    }
}

void MainWindow::closeFile() {
    m_database->close();
    onNoteChanged(0);
    m_notetaking->clear();
    setCurrentFile();
}

void MainWindow::showPreferences() {
    using namespace SettingsKey;

    Preferences::Data data;
    data.language = Settings::value<General::Language>();
    data.backupsDirectory = Settings::value<Backups::Directory>();

    data.fontFamily = Settings::value<SettingsKey::Editor::FontFamily>();
    data.fontSize = Settings::value<SettingsKey::Editor::FontSize>();

    data.hideTrayIcon = Settings::value<General::HideTrayIcon>();
    data.minimizeOnStartup = Settings::value<General::MinimizeOnStartup>();

    data.hotKeyEnabled = Settings::value<SettingsKey::GlobalHotkey::Enabled>();
    data.hotKey = Settings::value<SettingsKey::GlobalHotkey::Hotkey>();

    data.serverEnabled = Settings::value<Server::Enabled>();
    data.token = Settings::value<Server::Token>();
    data.port = Settings::value<Server::Port>();
    data.certificate = Settings::value<Server::Certificate>();
    data.privateKey = Settings::value<Server::PrivateKey>();

    Preferences preferences(data);

    if (preferences.exec() == QDialog::Accepted) {
        Preferences::Data data = preferences.data();

        Settings::setValue<General::Language>(data.language);
        Settings::setValue<Backups::Directory>(data.backupsDirectory);

        Settings::setValue<SettingsKey::Editor::FontFamily>(data.fontFamily);
        Settings::setValue<SettingsKey::Editor::FontSize>(data.fontSize);

        Settings::setValue<General::HideTrayIcon>(data.hideTrayIcon);
        Settings::setValue<General::MinimizeOnStartup>(data.minimizeOnStartup);

        Settings::setValue<SettingsKey::GlobalHotkey::Enabled>(data.hotKeyEnabled);
        Settings::setValue<SettingsKey::GlobalHotkey::Hotkey>(data.hotKey);

        Settings::setValue<Server::Enabled>(data.serverEnabled);
        Settings::setValue<Server::Token>(data.token);
        Settings::setValue<Server::Port>(data.port);
        Settings::setValue<Server::Certificate>(data.certificate);
        Settings::setValue<Server::PrivateKey>(data.privateKey);

        applyHotSettings();
    }
}

void MainWindow::find() {
    FindText findText;
    if (findText.exec() == QDialog::Rejected) return;

    m_findText = findText.text();

    QTextCursor cursor = m_editor->textCursor();
    QTextCursor savedCursor = cursor;

    cursor.movePosition(QTextCursor::Start);\
    m_editor->setTextCursor(cursor);

    if (!m_editor->find(m_findText)) {
        QMessageBox::warning(this, Const::App::Name, tr("Text not found"));
        m_editor->setTextCursor(savedCursor);
        return;
    }

    m_findNextAction->setEnabled(true);
    m_findPreviousAction->setEnabled(true);
}

void MainWindow::findNext() {
    m_editor->find(m_findText);
}

void MainWindow::findPrevious() {
    m_editor->find(m_findText, QTextDocument::FindBackward);
}

void MainWindow::showBirthdays() {
    auto birthdays = new Birthdays(m_database);
    birthdays->show();
}

void MainWindow::about() {
    using namespace Const::App;

    QMessageBox::about(this, tr("About %1").arg(Name),
        tr("<h3>%1 %2 %3</h3>"
           "Note-taking for quick notes<br><br>"
           "Based on Qt %4<br>"
           "Build on %5 %6<br><br>"
           "<a href=%7>%7</a><br><br>Copyright © %8, Vladimir Zarypov")
            .arg(Name, Version, Status, QT_VERSION_STR, BuildDate, BuildTime, URL, CopyrightYear));
}

void MainWindow::onNoteChanged(Id id) {
    m_editor->setId(id);
    m_editor->setEnabled(id > 0);

    m_findNextAction->setEnabled(false);
    m_findPreviousAction->setEnabled(false);

    if (id) {
        QString note = m_database->noteValue(id, "note").toString();
        m_editor->setPlainText(note);
        m_editor->setFocus();

        int line = m_database->noteValue(id, "line").toInt();
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
        m_database->updateNoteValue(lastId, "note", m_editor->document()->toPlainText());
    }

    m_database->updateNoteValue(lastId, "line", m_editor->textCursor().blockNumber());
}

void MainWindow::onGlobalActivated() {
    show();
    raise();
    activateWindow();
}
