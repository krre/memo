#pragma once
#include <QObject>
#include <QString>

class NoteTaking;
class Database;

class Exporter : public QObject {
    Q_OBJECT
public:
    static void exportAll(const QString& filePath, NoteTaking* noteTaking, Database* database, QWidget* parent);

private:
    static void exportBirthdays(const QString& dirPath, Database* database);
    static void compressDir(const QString& dirPath);
};
