#include "staticfunctions_files.h"


unsigned long long STATIC_FUNCTIONS::evaluateFileCount(const std::vector<string> &paths)
{
    unsigned long long cntr = 0;
    for(const auto& path: paths)
    {
        QFileInfo info(QString::fromStdString(path));
        cntr += static_cast<unsigned long long>(StaticFunctions::evaluateContentCount(info));
    }
    return cntr;
}

//--------------------------------------------------------------------------------------------------

// askUserForNoneExistingFilePath -> name ist irrefuehrend: path wird gegeben -> falls path existiert, wird lediglich vom user ein anderer fileName gefragt.
// Directory des absFilePath bleibt erhalten!
// falls ein Directory als path uebergeben wird, wird vom user ein directory-name verlangt, der noch nicht existiert, d.h.: basePath(path) + userDirName
QString STATIC_FUNCTIONS::askUserForNoneExistingFilePath(const QString &path, std::function<bool (const QString&)> fileNameValidator)
{
    QFileInfo fi(path);

    QString tarPath = path;
    QString tarBasePath = PATH::getBasePath(tarPath);

    if(fi.exists())
    {
        int cntr = 0, LOOP_MAX = 3;

        auto filePathValidator = [=](const QString& fileName){
            QString newAbsPath = tarBasePath + QDir::separator() + fileName;
            if( QFileInfo(newAbsPath).exists() )
                return false;

            return fileNameValidator(fileName);
        };

        while(fi.exists())
        {
            QString newFileName = genStringGetterDialog( "File does already exist!",
                                                        "'" + fi.fileName() + " does already exists - please choose another file name:",
                                                         fi.fileName(),
                                                         filePathValidator);
            tarPath = PATH::getBasePath(tarPath) + QDir::separator() + newFileName;

            fi = QFileInfo(tarPath);

            if(++cntr >= LOOP_MAX)
                break;
        }
        return cntr < LOOP_MAX ? tarPath : "";
    }else
        return path;
}

QString STATIC_FUNCTIONS::genStringGetterDialog(const QString &headline, const QString &message, const QString &initInput, std::function<bool (const QString &)> stringValidator)
{
    QString retVal = "";

    bool ok;

    QString adapted_message = message;

    QString text;
    do{
        text = QInputDialog::getText(nullptr,
                                     headline,
                                     adapted_message,
                                     QLineEdit::Normal,
                                     initInput, &ok);

        adapted_message = "invalid selection: '" + text + "': " + message;
    }while(!stringValidator(text));

    if (ok)
        retVal = text;

    return retVal;
}

//--------------------------------------------------------------------------------------------------

Process* STATIC_FUNCTIONS::execPythonScript(const QString &scriptPath, const QVector<QString> &args, bool waitForFinished)
{
    Process* p = new Process();
    QStringList params;

    params << scriptPath;
    for(const auto& arg: args)
        params << arg;


    QString scriptDir = QFileInfo(scriptPath).path();
//    p->setWorkingDirectory(scriptDir); // setWorkingDirectory verlangt einen absoluten path. scriptPath ist abund zu aber ein relativer und damit schlaegt setWorkingDirectory fehl -> da nicht relevant nur eine unnoetige zusaetzliche fehlerquelle

    p->start("python", params);
    if(waitForFinished)
    {
        p->waitForFinished(-1);
        QString p_stdout = p->readAll();
        qDebug() << p_stdout;
        return nullptr;
    }else
        return p;
}

Process* STATIC_FUNCTIONS::execPythonScript(const string &scriptPath, const std::vector<string> &args, bool waitForFinished)
{
    QVector<QString> q_args;
    for(const auto& arg: args)
        q_args.push_back(QString::fromStdString(arg));

    return execPythonScript(QString::fromStdString(scriptPath), q_args, waitForFinished);
}

//--------------------------------------------------------------------------------------------------


QString STATIC_FUNCTIONS::getUniqueFilePath(const QString &absFilePath)
{
    QFileInfo fi(absFilePath);

    if( !fi.exists() )
        return absFilePath;

    QString basePath = PATH::getBasePath(absFilePath);
    QString fileName_withoutExt = PATH::getFileNameWithoutExtension(absFilePath);
    QString ext = fi.completeSuffix();

    // QFileInfo::completeSuffix gibt die suffix ohne den fuehrenden punkt an:
    if( !ext.isEmpty() )
        ext.prepend('.');

    int cntr = 2;

    QString path = "";

    do{
        QString path = QString("%1%2%3_%4%5").arg(basePath)
                                             .arg(QDir::separator())
                                             .arg(fileName_withoutExt)
                                             .arg(cntr++)
                                             .arg(ext);
        fi = QFileInfo(path);
    }while(fi.exists());

    return path;
}

QString STATIC_FUNCTIONS::getUniqueFolderPath(const QString &absFolderPath)
{
    QFileInfo fi(absFolderPath);

    if( !fi.exists() )
        return absFolderPath;

    QString basePath = PATH::getBasePath(absFolderPath);
    QString fileName_withoutExt = fi.fileName();

    int cntr = 2;

    QString path = "";

    do{
        QString path = QString("%1%2%3_%4").arg(basePath)
                                           .arg(QDir::separator())
                                           .arg(fileName_withoutExt)
                                           .arg(cntr++);
        fi = QFileInfo(path);
    }while(fi.exists());

    return path;
}

//--------------------------------------------------------------------------------------------------


void STATIC_FUNCTIONS::openTerminal(QString sourceDir)
{
    OS local_os = LOCAL_OS();
    if(local_os == OS::LINUX)
    {
        Process* proc = new Process();
        proc->setWorkingDirectory(sourceDir);
        proc->start("gnome-terminal");
    }else if (local_os == OS::WINDOWS)
    {
        Process* proc = new Process();
        proc->setWorkingDirectory(sourceDir);
        proc->start("cmd.exe");
    }else
        throw "STATIC_FUNCTIONS::openTerminal -> OS not supported!";
}

//--------------------------------------------------------------------------------------------------


bool STATIC_FUNCTIONS::isSubDirectory(const string &potentialSubFile, const string &potentialParentFolder)
{
    return isSubDirectory(QFileInfo(QString::fromStdString(potentialSubFile)), QFileInfo(QString::fromStdString(potentialParentFolder)));
}

bool STATIC_FUNCTIONS::isSubDirectory(const QString& potentialSubFile, const QString& potentialParentFolder)
{
    return isSubDirectory(QFileInfo(potentialSubFile), QFileInfo(potentialParentFolder));
}
bool STATIC_FUNCTIONS::isSubDirectory(const QFileInfo& potentialSubFile, const QFileInfo& potentialParentFolder)
{
    // eine datei kann kein parent-folder sein von irgendwas:
    if(potentialParentFolder.isFile())
        return false;

    QDir parentFolder   = PATH::getDirFromPath(potentialParentFolder);
    QDir subFilesFolder = PATH::getDirFromPath(potentialSubFile);

    int parentFolderPathLength = parentFolder.absolutePath().length();

    do{
        if(subFilesFolder == parentFolder)
            return true;
    }while(subFilesFolder.cdUp() &&
           subFilesFolder.absolutePath().length() >= parentFolderPathLength);


    return false;
}

//--------------------------------------------------------------------------------------------------

#include "file_handling_src/fileinfobd.h"

void STATIC_FUNCTIONS::removeSubDirsIfParentDirIsInContainer(std::vector<FileInfoBD*>& dirs_to_validate)
{
    std::unordered_map<std::string, FileInfoBD*> path_to_dir;
    std::vector<std::string> paths;
    for(auto* dir: dirs_to_validate)
    {
        std::string absPath = dir->absPath();
        path_to_dir[absPath] = dir;
        paths.push_back(absPath);
    }

    bool invalidPathDetected = removeSubDirsIfParentDirIsInContainer(paths);

    if(invalidPathDetected)
    {
        dirs_to_validate.clear();
        for(const auto& path: paths)
        {
            dirs_to_validate.push_back( path_to_dir[path] );
        }
    }
}

bool STATIC_FUNCTIONS::removeSubDirsIfParentDirIsInContainer(std::vector<std::string>& paths_to_validate)
{
    bool invalidPathDetected = false;

    for(unsigned long i=0; i < paths_to_validate.size(); ++i)
    {
        for(unsigned long j=0; j < paths_to_validate.size(); ++j)
        {
            if(i != j)
            {
                bool isSubDir = STATIC_FUNCTIONS::isSubDirectory(paths_to_validate[i], paths_to_validate[j]);
                if(isSubDir)
                {
                    invalidPathDetected = true;

                    paths_to_validate.erase(paths_to_validate.begin() + static_cast<int>(i));
                    --i;
                    if(j > i)
                        --j;
                }else{
                    isSubDir = STATIC_FUNCTIONS::isSubDirectory(paths_to_validate[j], paths_to_validate[i]);
                    if(isSubDir)
                    {
                        invalidPathDetected = true;

                        paths_to_validate.erase(paths_to_validate.begin() + static_cast<int>(j));
                        --j;
                        if(i > j)
                        {
                            --i;
                        }
                    }
                }
            }
        }
    }

    return invalidPathDetected;
}

//----------------------------------------------------------------------------------

OS STATIC_FUNCTIONS::LOCAL_OS()
{
    QString os = QSysInfo::kernelType();
    if(os.contains("win"))
        return OS::WINDOWS;
    else if(os == "linux")
        return OS::LINUX;
    else if(os == "darwin")
        return OS::MACOS;
    else
        return OS::UNKNOWN;
}

