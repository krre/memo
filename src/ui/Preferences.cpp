#include "Preferences.h"
#include <QtWidgets>
#include <QtNetwork>

Preferences::Preferences(const Data& data, QWidget* parent) : StandardDialog(parent) {
    setWindowTitle(tr("Preferences"));

    auto layout = new QVBoxLayout;
    layout->addWidget(createUiGroupBox(data));
    layout->addWidget(createHotkeyGroupBox(data));
    layout->addWidget(createBackupsGroupBox(data));
    layout->addWidget(createServerGroupBox(data));
    layout->addStretch(1);

    setContentLayout(layout);
    resizeToWidth(600);

    m_language = data.language;
}

Preferences::Data Preferences::data() const {
    Data result;

    result.language = m_languageComboBox->currentData().toString();
    result.backupsDirectory = m_backupsLineEdit->text();

    result.minimizeOnStartup = m_minimizeCheckBox->isChecked();
    result.hideTrayIcon = m_hideTrayCheckBox->isChecked();

    result.fontFamily = m_fontFamilyLineEdit->text();
    result.fontSize = m_fontSizeLineEdit->text().toInt();

    result.hotKey = m_hotkeyLineEdit->text();
    result.hotKeyEnabled = m_hotkeyGroupBox->isChecked();

    result.serverEnabled = m_serverGroupBox->isChecked();
    result.port = m_portLineEdit->text().toInt();
    result.token = m_tokenLineEdit->text();
    result.certificate = m_certificateLineEdit->text();
    result.privateKey = m_privateKeyEdit->text();

    return result;
}

void Preferences::accept() {
    QString language = m_languageComboBox->currentData().toString();

    if (language != m_language) {
        QMessageBox::information(this, tr("Restart requred"), tr("You must restart application"));
    }

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
    QString dirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select Directory"), m_backupsLineEdit->text());

    if (!dirPath.isEmpty()) {
        m_backupsLineEdit->setText(dirPath);
    }
}

void Preferences::onCertificateBrowseButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Select SSL Certificate"), m_certificateLineEdit->text());

    if (!filePath.isEmpty()) {
        m_certificateLineEdit->setText(filePath);
    }
}

void Preferences::onPrivateKeyBrowseButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, tr("Select SSL Private Key"), m_privateKeyEdit->text());

    if (!filePath.isEmpty()) {
        m_privateKeyEdit->setText(filePath);
    }
}

QGroupBox* Preferences::createUiGroupBox(const Data& data) {
    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem(tr("English"), "en");
    m_languageComboBox->addItem(tr("Russian"), "ru");

    if (!data.language.isEmpty()) {
        int index = m_languageComboBox->findData(data.language);

        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);
    m_fontFamilyLineEdit->setText(data.fontFamily);

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);
    m_fontSizeLineEdit->setText(QString::number(data.fontSize));

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Preferences::onFontButtonClicked);

    auto fontLayout = new QHBoxLayout;
    fontLayout->addWidget(m_fontFamilyLineEdit);
    fontLayout->addWidget(m_fontSizeLineEdit);
    fontLayout->addWidget(fontButton);

    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    m_minimizeCheckBox->setChecked(data.minimizeOnStartup);

    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));
    m_hideTrayCheckBox->setChecked(data.hideTrayIcon);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("Language:"), m_languageComboBox);
    formLayout->addRow(tr("Font:"), fontLayout);

    formLayout->itemAt(formLayout->indexOf(m_languageComboBox))->setAlignment(Qt::AlignLeft);

    auto result = new QGroupBox(tr("User Interface"));
    auto verticalLayout = new QVBoxLayout(result);
    verticalLayout->addLayout(formLayout);
    verticalLayout->addWidget(m_minimizeCheckBox);
    verticalLayout->addWidget(m_hideTrayCheckBox);

    return result;
}

QGroupBox* Preferences::createHotkeyGroupBox(const Data& data) {
    m_hotkeyGroupBox = new QGroupBox(tr("Global Hotkey"));
    m_hotkeyGroupBox->setCheckable(true);
    m_hotkeyGroupBox->setChecked(data.hotKeyEnabled);

    m_hotkeyLineEdit = new QLineEdit;
    m_hotkeyLineEdit->setText(data.hotKey);

    auto hotkeyLayout = new QVBoxLayout(m_hotkeyGroupBox);
    hotkeyLayout->addWidget(m_hotkeyLineEdit);

    return m_hotkeyGroupBox;
}

QGroupBox* Preferences::createBackupsGroupBox(const Data& data) {
    auto label = new QLabel(tr("Directory:"));
    m_backupsLineEdit = new QLineEdit;
    m_backupsLineEdit->setText(data.backupsDirectory);

    auto browseButton = new QPushButton(tr("Browse..."));
    connect(browseButton, &QPushButton::clicked, this, &Preferences::onBackupsBrowseButtonClicked);

    auto result = new QGroupBox(tr("Backups"));
    auto horizontalLayout = new QHBoxLayout(result);
    horizontalLayout->addWidget(label);
    horizontalLayout->addWidget(m_backupsLineEdit);
    horizontalLayout->addWidget(browseButton);
    return result;
}

QGroupBox* Preferences::createServerGroupBox(const Data& data) {
    QString addresses;

    for (const QHostAddress& address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address.isGlobal()) {
            addresses += address.toString() + ";";
        }
    }

    addresses.chop(1);

    auto addressLineEdit = new QLineEdit(addresses);
    addressLineEdit->setReadOnly(true);

    m_portLineEdit = new QLineEdit;
    m_portLineEdit->setText(QString::number(data.port));

    m_tokenLineEdit = new QLineEdit;
    m_tokenLineEdit->setText(data.token);

    m_certificateLineEdit = new QLineEdit;
    m_certificateLineEdit->setText(data.certificate);

    auto certificateBrowseButton = new QPushButton(tr("Browse..."));
    connect(certificateBrowseButton, &QPushButton::clicked, this, &Preferences::onCertificateBrowseButtonClicked);

    m_privateKeyEdit = new QLineEdit;
    m_privateKeyEdit->setText(data.privateKey);

    auto privateKeyBrowseButton = new QPushButton(tr("Browse..."));
    connect(privateKeyBrowseButton, &QPushButton::clicked, this, &Preferences::onPrivateKeyBrowseButtonClicked);

    auto certificateLayout = new QHBoxLayout;
    certificateLayout->addWidget(m_certificateLineEdit);
    certificateLayout->addWidget(certificateBrowseButton);

    auto privateKeyLayout = new QHBoxLayout;
    privateKeyLayout->addWidget(m_privateKeyEdit);
    privateKeyLayout->addWidget(privateKeyBrowseButton);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("IP address:"), addressLineEdit);
    formLayout->addRow(tr("Port:"), m_portLineEdit);
    formLayout->addRow(tr("Token:"), m_tokenLineEdit);
    formLayout->addRow(tr("SSL certificate:"), certificateLayout);
    formLayout->addRow(tr("SSL private key:"), privateKeyLayout);

    m_serverGroupBox = new QGroupBox(tr("Server"));
    m_serverGroupBox->setCheckable(true);
    m_serverGroupBox->setChecked(data.serverEnabled);
    m_serverGroupBox->setLayout(formLayout);

    return m_serverGroupBox;
}
