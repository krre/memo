#include "Preferences.h"
#include "BrowseLayout.h"
#include "settings/Settings.h"
#include "core/Application.h"
#include <QGroupBox>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFontDialog>
#include <QFileDialog>
#include <QFormLayout>
#include <QHostAddress>
#include <QNetworkInterface>

Preferences::Preferences(Settings* settings, QWidget* parent)
    : StandardDialog(parent), m_settings(settings) {
    setWindowTitle(tr("Preferences"));

    auto layout = new QVBoxLayout;
    layout->addWidget(createUiGroupBox());
    layout->addWidget(createHotkeyGroupBox());
    layout->addWidget(createBackupsGroupBox());
    layout->addWidget(createServerGroupBox());
    layout->addStretch(1);

    setContentLayout(layout);
    resizeToWidth(600);
}

void Preferences::accept() {
    QString language = m_languageComboBox->currentData().toString();

    if (m_settings->applicationLanguage() != language) {
        m_settings->setApplicationLanguage(m_languageComboBox->currentData().toString());
        QMessageBox::information(this, Application::Name, tr("You must restart application"));
    }

    m_settings->setApplicationMinimizeOnStartup(m_minimizeCheckBox->isChecked());
    m_settings->setApplicationHideTrayIcon(m_hideTrayCheckBox->isChecked());
    m_settings->setBirthdaysRemind(m_remindBirthdaysCheckBox->isChecked());
    m_settings->setBackupsDirectory(m_backupsBrowseLayout->text());

    m_settings->setEditorFontFamily(m_fontFamilyLineEdit->text());
    m_settings->setEditorFontSize(m_fontSizeLineEdit->text().toInt());
    m_settings->setEditorShowSymbolsCount(m_showSymbolsCountCheckBox->isChecked());

    m_settings->setGlobalHotkeyEnabled(m_hotkeyGroupBox->isChecked());
    m_settings->setGlobalHotkeyValue(m_hotkeyLineEdit->text());

    m_settings->setServerEnabled(m_serverGroupBox->isChecked());
    m_settings->setServerPort(m_portLineEdit->text().toInt());
    m_settings->setServerToken(m_tokenLineEdit->text());
    m_settings->setServerSslEnabled(m_sslGroupBox->isChecked());
    m_settings->setServerCertificate(m_certificateBrowseLayout->text());
    m_settings->setServerPrivateKey(m_privateKeyBrowseLayout->text());

    QDialog::accept();
}

void Preferences::onFontButtonClicked() {
    QFont currentFont = font();
    currentFont.setFamily(m_fontFamilyLineEdit->text());
    currentFont.setPointSize(m_fontSizeLineEdit->text().toInt());

    bool ok;
    QFont font = QFontDialog::getFont(&ok, currentFont, this, tr("Select Font"));

    if (ok) {
        m_fontFamilyLineEdit->setText(font.family());
        m_fontSizeLineEdit->setText(QString::number(font.pointSize()));
    }
}

void Preferences::onBackupsBrowseButtonClicked() {
    QString dirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), m_backupsBrowseLayout->text());

    if (!dirPath.isEmpty()) {
        m_backupsBrowseLayout->setText(dirPath);
    }
}

QGroupBox* Preferences::createUiGroupBox() {
    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Russian"), "ru");

    QString lang = m_settings->applicationLanguage();

    if (!lang.isEmpty()) {
        int index = m_languageComboBox->findData(lang);

        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);
    m_fontFamilyLineEdit->setText(m_settings->editorFontFamily());

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);
    m_fontSizeLineEdit->setText(QString::number(m_settings->editorFontSize()));

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Preferences::onFontButtonClicked);

    auto fontLayout = new QHBoxLayout;
    fontLayout->addWidget(m_fontFamilyLineEdit);
    fontLayout->addWidget(m_fontSizeLineEdit);
    fontLayout->addWidget(fontButton);

    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    m_minimizeCheckBox->setChecked(m_settings->applicationMinimizeOnStartup());

    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));
    m_hideTrayCheckBox->setChecked(m_settings->applicationHideTrayIcon());

    m_remindBirthdaysCheckBox = new QCheckBox(tr("Remind about birthdays"));
    m_remindBirthdaysCheckBox->setChecked(m_settings->birthdaysRemind());

    m_showSymbolsCountCheckBox = new QCheckBox(tr("Show symbols count"));
    m_showSymbolsCountCheckBox->setChecked(m_settings->editorShowSymbolsCount());

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Language:"), m_languageComboBox);
    formLayout->addRow(tr("Font:"), fontLayout);

    formLayout->itemAt(formLayout->indexOf(m_languageComboBox))->setAlignment(Qt::AlignLeft);

    auto result = new QGroupBox(tr("User Interface"));
    auto verticalLayout = new QVBoxLayout(result);
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_minimizeCheckBox);
    verticalLayout->addWidget(m_hideTrayCheckBox);
    verticalLayout->addWidget(m_remindBirthdaysCheckBox);
    verticalLayout->addWidget(m_showSymbolsCountCheckBox);

    return result;
}

QGroupBox* Preferences::createHotkeyGroupBox() {
    m_hotkeyGroupBox = new QGroupBox(tr("Global Hotkey"));
    m_hotkeyGroupBox->setCheckable(true);
    m_hotkeyGroupBox->setChecked(m_settings->globalHotkeyEnabled());

    m_hotkeyLineEdit = new QLineEdit;
    m_hotkeyLineEdit->setText(m_settings->globalHotkeyValue());

    auto hotkeyLayout = new QVBoxLayout(m_hotkeyGroupBox);
    hotkeyLayout->addWidget(m_hotkeyLineEdit);

    return m_hotkeyGroupBox;
}

QGroupBox* Preferences::createBackupsGroupBox() {
    m_backupsBrowseLayout = new BrowseLayout(BrowseLayout::Mode::Directory, m_settings->backupsDirectory());

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Directory:"), m_backupsBrowseLayout);

    auto result = new QGroupBox(tr("Backups"));
    result->setLayout(formLayout);

    return result;
}

QGroupBox* Preferences::createServerGroupBox() {
    QString addresses;
    const auto allAddresses = QNetworkInterface::allAddresses();

    for (const QHostAddress& address: allAddresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isGlobal()) {
            addresses += address.toString() + ";";
        }
    }

    addresses.chop(1);

    auto addressLineEdit = new QLineEdit(addresses);
    addressLineEdit->setReadOnly(true);

    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setText(QString::number(m_settings->serverPort()));

    m_tokenLineEdit = new QLineEdit;
    m_tokenLineEdit->setText(m_settings->serverToken());

    m_certificateBrowseLayout = new BrowseLayout(BrowseLayout::Mode::File, m_settings->serverCertificate());
    m_privateKeyBrowseLayout = new BrowseLayout(BrowseLayout::Mode::File, m_settings->serverPrivateKey());

    auto sslFormLayout = new QFormLayout;
    sslFormLayout->addRow(tr("Certificate:"), m_certificateBrowseLayout);
    sslFormLayout->addRow(tr("Private key:"), m_privateKeyBrowseLayout);

    m_sslGroupBox = new QGroupBox("SSL");
    m_sslGroupBox->setCheckable(true);
    m_sslGroupBox->setChecked(m_settings->serverSslEnabled());
    m_sslGroupBox->setLayout(sslFormLayout);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("IP address:"), addressLineEdit);
    formLayout->addRow(tr("Port:"), m_portLineEdit);
    formLayout->addRow(tr("Token:"), m_tokenLineEdit);

    auto verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_sslGroupBox);

    m_serverGroupBox = new QGroupBox(tr("Server"));
    m_serverGroupBox->setCheckable(true);
    m_serverGroupBox->setChecked(m_settings->serverEnabled());
    m_serverGroupBox->setLayout(verticalLayout);

    return m_serverGroupBox;
}
