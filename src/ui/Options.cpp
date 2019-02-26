#include "Options.h"
#include <QtWidgets>

Options::Options(QWidget* parent) : QDialog (parent) {
    setWindowTitle(tr("Options"));
    QVBoxLayout* verticalLayout = new QVBoxLayout;

    QGroupBox* groupBox = new QGroupBox(tr("User Interface"));

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel(tr("Language:")), 0, 0);

    languageComboBox = new QComboBox;
    languageComboBox->addItem(tr("<System>"));
    languageComboBox->addItem("English", "en");
    languageComboBox->addItem("Russian", "ru");

    gridLayout->addWidget(languageComboBox, 0, 1, Qt::AlignLeft);
    gridLayout->setColumnStretch(1, 1);

    groupBox->setLayout(gridLayout);

    verticalLayout->addWidget(groupBox);
    verticalLayout->addStretch(1);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    verticalLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(verticalLayout);

    resize(500, 300);

    readSettings();
}

void Options::accept() {
    if (writeSettings()) {
        QMessageBox::information(this, tr("Restart requred"), tr("You should be restart application"));
    }

    close();
}

void Options::readSettings() {
    QSettings settings;
    QString language = settings.value("language").toString();

    if (!language.isEmpty()) {
        int index = languageComboBox->findData(language);
        if (index != -1) {
            languageComboBox->setCurrentIndex(index);
        }
    }
}

bool Options::writeSettings() {
    bool restartRequre = false;

    QSettings settings;

    QString language = languageComboBox->currentData().toString();
    if (language != settings.value("language").toString()) {
        restartRequre = true;
    }

    settings.setValue("language", language);

    return restartRequre;
}
