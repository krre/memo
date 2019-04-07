#pragma once
#include <QDialog>

class ProjectSettings;
class QNetworkAccessManager;
class QNetworkReply;
class QFile;
class QLineEdit;
class QProgressBar;

class UploadDialog : public QDialog {
    Q_OBJECT
public:

    enum class FileType {
        Manifest,
        Zip
    };

    explicit UploadDialog(FileType type, const QString& filePath, ProjectSettings* settings, QWidget* parent = nullptr);

public slots:
    void accept() override;

private slots:
    void uploadFinished(QNetworkReply* reply);
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

private:
    FileType fileType;
    QString filePath;
    ProjectSettings* projectSettings;

    QNetworkAccessManager* manager = nullptr;
    QFile* file = nullptr;

    QLineEdit* urlLineEdit = nullptr;
    QLineEdit* loginLineEdit = nullptr;
    QLineEdit* passwordLineEdit = nullptr;
    QProgressBar* progressBar = nullptr;
};
