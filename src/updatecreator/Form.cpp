#include "Form.h"
#include <QtWidgets>

Form::Form(QWidget* parent) : QWidget(parent) {
    auto layout = new QVBoxLayout;
    setLayout(layout);

    auto generalGroupBox = new QGroupBox(tr("General"));

    auto generalGridLayout = new QGridLayout;
    generalGroupBox->setLayout(generalGridLayout);

    generalGridLayout->addWidget(new QLabel(tr("Path:")), 0, 0);
    manifestLabel = new QLabel;
    generalGridLayout->addWidget(manifestLabel, 0, 1);

    generalGridLayout->addWidget(new QLabel(tr("URL:")), 1, 0);
    urlLineEdit = new QLineEdit;
    generalGridLayout->addWidget(urlLineEdit, 1, 1);

    layout->addWidget(generalGroupBox);

    auto updateGroupBox = new QGroupBox(tr("Update"));

    auto updateGridLayout = new QGridLayout;
    updateGroupBox->setLayout(updateGridLayout);

    updateGridLayout->addWidget(new QLabel(tr("Version:")), 0, 0);
    versionLineEdit = new QLineEdit;
    updateGridLayout->addWidget(versionLineEdit, 0, 1);

    updateGridLayout->addWidget(new QLabel(tr("Date:")), 1, 0);
    dateLineEdit = new QLineEdit;
    updateGridLayout->addWidget(dateLineEdit, 1, 1);

    updateGridLayout->addWidget(new QLabel(tr("OS:")), 2, 0);

    auto osLayout = new QHBoxLayout;
    windowsCheckBox = new QCheckBox("Windows");
    osLayout->addWidget(windowsCheckBox);

    linuxCheckBox = new QCheckBox("Linux");
    osLayout->addWidget(linuxCheckBox);

    macosCheckBox = new QCheckBox("MacOS");
    osLayout->addWidget(macosCheckBox);

    updateGridLayout->addLayout(osLayout, 2, 1, Qt::AlignLeft);

    updateGridLayout->addWidget(new QLabel("Channel:"), 3, 0);
    channelComboBox = new QComboBox;
    channelComboBox->addItem("release");
    channelComboBox->addItem("beta");
    updateGridLayout->addWidget(channelComboBox, 3, 1, Qt::AlignLeft);

    updateGridLayout->addWidget(new QLabel(tr("Size:")), 4, 0);
    sizeLabel = new QLabel;
    updateGridLayout->addWidget(sizeLabel, 4, 1);

    updateGridLayout->addWidget(new QLabel(tr("Description:")), 5, 0);
    descriptionTextEdit = new QPlainTextEdit;
    updateGridLayout->addWidget(descriptionTextEdit, 5, 1);

    updateGridLayout->setRowStretch(2, 0);

    layout->addWidget(updateGroupBox);
    layout->setStretch(1, 0);

    connect(qApp, &QApplication::focusChanged, this, &Form::onFocusChanged);
}

void Form::populateUpdate(const ListModel::Update& update) {
    versionLineEdit->setText(update.version);
    dateLineEdit->setText(update.date);

    windowsCheckBox->setChecked(update.os.contains("windows"));
    linuxCheckBox->setChecked(update.os.contains("linux"));
    macosCheckBox->setChecked(update.os.contains("macos"));

    int channelIndex = channelComboBox->findText(update.channel);
    channelComboBox->setCurrentIndex(channelIndex);

    sizeLabel->setText(QString::number(update.size));
    fileSize = update.size;
    descriptionTextEdit->setPlainText(update.description);
}

ListModel::Update Form::getUpdate() const {
    ListModel::Update update;
    update.version = versionLineEdit->text();
    update.date = dateLineEdit->text();

    if (windowsCheckBox->isChecked()) {
        update.os.append("windows");
    }

    if (linuxCheckBox->isChecked()) {
        update.os.append("linux");
    }

    if (macosCheckBox->isChecked()) {
        update.os.append("macos");
    }

    update.channel = channelComboBox->currentText();
    update.size = fileSize;
    update.description = descriptionTextEdit->document()->toPlainText();

    return update;
}

void Form::setManifestPath(const QString path) {
    manifestLabel->setText(path);
}

void Form::setUrl(const QString& url) {
    urlLineEdit->setText(url);
}

QString Form::getUrl() const {
    return urlLineEdit->text();
}

void Form::clear() {
    urlLineEdit->clear();
    versionLineEdit->clear();
    dateLineEdit->clear();
    windowsCheckBox->setChecked(false);
    linuxCheckBox->setChecked(false);
    macosCheckBox->setChecked(false);
    channelComboBox->setCurrentIndex(0);
    sizeLabel->clear();
    descriptionTextEdit->clear();
}

void Form::onFocusChanged(QWidget* from, QWidget* to) {
    Q_UNUSED(to)

    QWidget* rootTo = root(to);
    QWidget* rootFrom = root(from);

    if (rootFrom == this && rootTo != this) {
        emit lostFocus();
    }
}

QWidget* Form::root(QWidget* child) {
    auto widget = child;

    while (widget != nullptr && widget != this) {
         widget = widget->parentWidget();
    }

    return widget;
}
