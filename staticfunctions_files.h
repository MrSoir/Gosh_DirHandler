#ifndef SATATICFUNCTIONS_FILES_H
#define SATATICFUNCTIONS_FILES_H

#include <QString>
#include <QVector>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QDebug>
#include <QObject>
#include <QSysInfo>

#include <vector>
#include <string>
#include <unordered_set>
#include <functional>

#include "pathmanipulation.h"
#include "staticfunctions.h"
#include "process.h"

class FileInfoBD;

//----------------------------------------------------------------------------------


enum OS
{
    LINUX,
    WINDOWS,
    MACOS,
    UNKNOWN
};


//----------------------------------------------------------------------------------


namespace STATIC_FUNCTIONS
{
    unsigned long long evaluateFileCount(const std::vector<std::string>& paths);

    // askUserForNoneExistingFilePath -> name ist irrefuehrend: path wird gegeben -> falls path existiert, wird lediglich vom user ein anderer fileName gefragt.
    // Directory des absFilePath bleibt erhalten!
    // falls ein Directory als path uebergeben wird, wird vom user ein directory-name verlangt, der noch nicht existiert, d.h.: basePath(path) + userDirName
    QString askUserForNoneExistingFilePath(const QString& path, std::function<bool(const QString& fileName)> fileNameValidator = [](const QString& fileName){Q_UNUSED(fileName) return true;});

    QString genStringGetterDialog(const QString &headline,
                                  const QString &message,
                                  const QString& initInput,
                                  std::function<bool (const QString &)> stringValidator = [](const QString& s){Q_UNUSED(s)return true;});

    Process* execPythonScript(const QString& scriptPath, const QVector<QString>& args, bool waitForFinished = true);
    Process* execPythonScript(const std::string& scriptPath, const std::vector<std::string>& args, bool waitForFinished = true);

//----------------------------------------------------------------------------------

    QString getUniqueFilePath(const QString& absFilePath);
    QString getUniqueFolderPath(const QString& absFolderPath);

//----------------------------------------------------------------------------------

    OS LOCAL_OS();

//----------------------------------------------------------------------------------

    void openTerminal(QString sourceDir);

//----------------------------------------------------------------------------------

    bool isSubDirectory(const std::string& potentialSubFile, const std::string& potentialParentFolder);
    bool isSubDirectory(const QString& potentialSubFile, const QString& potentialParentFolder);
    bool isSubDirectory(const QFileInfo& potentialSubFile, const QFileInfo& potentialParentFolder);

//----------------------------------------------------------------------------------

    void removeSubDirsIfParentDirIsInContainer(std::vector<FileInfoBD*>& dirs_to_validate);
    bool removeSubDirsIfParentDirIsInContainer(std::vector<std::string>& paths_to_validate);
}

#endif // SATATICFUNCTIONS_FILES_H
