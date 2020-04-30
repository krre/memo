#include "Options.h"
#include "core/Constants.h"
#include <QtWidgets>

Options::Options(QWidget* parent) : QDialog (parent) {
    setWindowTitle(tr("Options"));
    auto layout = new QVBoxLayout;

    auto groupBoxUi = new QGroupBox(tr("User Interface"));

    auto gridLayoutUi = new QGridLayout;
    gridLayoutUi->setColumnStretch(1, 1);

    // Language
    gridLayoutUi->addWidget(new QLabel(tr("Language:")), 0, 0);

    m_languageComboBox = new QComboBox;
    m_languageComboBox->addItem(tr("<System>"));
    m_languageComboBox->addItem("English", "en");
    m_languageComboBox->addItem("Russian", "ru");

    gridLayoutUi->addWidget(m_languageComboBox, 0, 1, Qt::AlignLeft);

    // Font
    gridLayoutUi->addWidget(new QLabel(tr("Font:")), 1, 0);
    auto fontLayout = new QHBoxLayout;

    m_fontFamilyLineEdit = new QLineEdit;
    m_fontFamilyLineEdit->setReadOnly(true);
    fontLayout->addWidget(m_fontFamilyLineEdit);

    m_fontSizeLineEdit = new QLineEdit;
    m_fontSizeLineEdit->setMaximumWidth(50);
    m_fontSizeLineEdit->setReadOnly(true);
    fontLayout->addWidget(m_fontSizeLineEdit);

    auto fontButton = new QPushButton(tr("Open..."));
    connect(fontButton, &QPushButton::clicked, this, &Options::openFontDialog);
    fontLayout->addWidget(fontButton);

    gridLayoutUi->addLayout(fontLayout, 1, 1);

    // Minimize to tray
    m_minimizeCheckBox = new QCheckBox(tr("Minimize to tray on startup"));
    gridLayoutUi->addWidget(m_minimizeCheckBox, 2, 0, 1, -1);

    // Hide tray icon
    m_hideTrayCheckBox = new QCheckBox(tr("Hide tray icon"));
    gridLayoutUi->addWidget(m_hideTrayCheckBox, 3, 0, 1, -1);

    groupBoxUi->setLayout(gridLayoutUi);

    layout->addWidget(groupBoxUi);

    // Global hotkey
    m_groupBoxHotkey = new QGroupBox(tr("Global Hotkey"));
    m_groupBoxHotkey->setCheckable(true);
    auto layoutHotkey = new QVBoxLayout;
    m_hotkeyLineEdit = new QLineEdit(m_groupBoxHotkey);
    layoutHotkey->addWidget(m_hotkeyLineEdit);
    m_groupBoxHotkey->setLayout(layoutHotkey);

    layout->addWidget(m_groupBoxHotkey);
    layout->addStretch(1);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(layout);

    resize(500, 300);

    readSettings();
}

void Options::accept() {
    if (writeSettings()) {
        QMessageBox::information(this, tr("Restart requred"), tr("You must restart application"));
    }

    QDialog::accept();
}

void Options::openFontDialog() {
    bool ok;
    QFont currentFont = font();
    currentFont.setFamily(m_fontFamilyLineEdit->text());
    currentFont.setPointSize(m_fontSizeLineEdit->text().toInt());

    QFont font = QFontDialog::getFont(&ok, currentFont, this, tr("Select Font"));
    if (ok) {
        m_fontFamilyLineEdit->setText(font.family());
        m_fontSizeLineEdit->setText(QString::number(font.pointSize()));
    }
}

void Options::readSettings() {
    QSettings settings;

    QString language = settings.value("language").toString();

    if (!language.isEmpty()) {
        int index = m_languageComboBox->findData(language);
        if (index != -1) {
            m_languageComboBox->setCurrentIndex(index);
        }
    }

    m_minimizeCheckBox->setChecked(settings.value("minimizeOnStartup").toBool());
    m_hideTrayCheckBox->setChecked(settings.value("hideTrayIcon").toBool());

    m_fontFamilyLineEdit->setText(settings.value("Editor/fontFamily", font().family()).toString());
    m_fontSizeLineEdit->setText(settings.value("Editor/fontSize", font().pointSize()).toString());

    m_hotkeyLineEdit->setText(settings.value("GlobalHotkey/hotkey", Const::DefaultSettings::GlobalHotkey).toString());
    m_groupBoxHotkey->setChecked(settings.value("GlobalHotkey/enabled", true).toBool());
}

bool Options::writeSettings() {
    bool restartRequre = false;

    QSettings settings;

    QString language = m_languageComboBox->currentData().toString();

    if (language != settings.value("language").toString()) {
        restartRequre = true;
    }

    settings.setValue("language", language);
    settings.setValue("minimizeOnStartup", m_minimizeCheckBox->isChecked());
    settings.setValue("hideTrayIcon", m_hideTrayCheckBox->isChecked());

    settings.setValue("Editor/fontFamily", m_fontFamilyLineEdit->text());
    settings.setValue("Editor/fontSize", m_fontSizeLineEdit->text());

    settings.setValue("GlobalHotkey/hotkey", m_hotkeyLineEdit->text());
    settings.setValue("GlobalHotkey/enabled", m_groupBoxHotkey->isChecked());

    return restartRequre;
}
