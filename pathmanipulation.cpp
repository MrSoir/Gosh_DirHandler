#include "pathmanipulation.h"

QString PATH::trimPathSeparatorLeft(const QString& path)
{
    if(path.isEmpty())
        return "";

    if(path[0] == QDir::separator())
        return path.right(path.size()-1);
    return path;
}
QString PATH::trimPathSeparatorRight(const QString& path)
{
    if(path.isEmpty())
        return "";

    if(path[path.size()-1] == QDir::separator())
        return path.left(path.size()-1);
    return path;
}
QString PATH::trimPathSeparator(const QString& path)
{
    return trimPathSeparatorRight(trimPathSeparatorLeft(path));
}

//-----------------------------------------------------------------------

QString PATH::getBasePath(const QString& path)
{
    QFileInfo fi(trimPathSeparatorRight(path));
    return fi.absolutePath();
}
QString PATH::getBasePath(const QFileInfo& fi)
{
    return getBasePath(fi.absoluteFilePath());
}

//-----------------------------------------------------------------------

QString PATH::getRelPathFragment(const QString& absPath, const QString& basePath)
{
    QString absPathTrmd = trimPathSeparatorRight(absPath);
    QString basePathTrmd = trimPathSeparatorRight(basePath);
    return trimPathSeparatorLeft( absPathTrmd.right( absPathTrmd.size() - basePathTrmd.size() ) );
}
QString PATH::getRelPathFragment(const QFileInfo& fi, const QString& basePath)
{
    return getRelPathFragment(fi.absoluteFilePath(), basePath);
}

//-----------------------------------------------------------------------

QString PATH::genTargetPath_hlpr(const QString& relPathFragment, const QString& tarDirPath)
{
    return trimPathSeparatorRight(tarDirPath) + QDir::separator() + trimPathSeparatorLeft(relPathFragment);
}

//-----------------------------------------------------------------------

QString PATH::genTargetPath(const QString& absPath, const QString& tarPath)
{
    QString basePath = getBasePath(absPath);
    return genTargetPath(absPath, basePath, tarPath);
}
QString PATH::genTargetPath(const QString& absPath, const QString& basePath, const QString& tarPath)
{
    QString relPathFragment = getRelPathFragment(absPath, basePath);
    return genTargetPath_hlpr(relPathFragment, tarPath);
}

QString PATH::genTargetPath(const std::string &absPath, const std::string &tarPath)
{
    QString basePath = getBasePath(QString::fromStdString(absPath));
    return genTargetPath(QString::fromStdString(absPath), basePath, QString::fromStdString(tarPath));
}
QString PATH::genTargetPath(const std::string &absPath, const std::string &basePath, const std::string &tarPath)
{
    QString relPathFragment = getRelPathFragment(QString::fromStdString(absPath), QString::fromStdString(basePath));
    return genTargetPath_hlpr(relPathFragment, QString::fromStdString(tarPath));
}

QString PATH::getFileNameWithoutExtension(const QString &absPath)
{
    QFileInfo fi(absPath);
    QString basePath = getBasePath(absPath);
    QString suffix = fi.completeSuffix();
    QString fileName = fi.fileName();

    int fn_sz = fileName.size();
    int sf_sz = suffix.size();

    if(sf_sz > fn_sz)
        return QString("");

    return fileName.left(fn_sz - sf_sz);
}

QString PATH::getBasePath(const std::string &path)
{
    return getBasePath(QString::fromStdString(path));
}

QString PATH::join(const QString &headPath, const QString &tailPath)
{
    QString trimmedHead = trimPathSeparatorRight(headPath);
    QString trimmedTail = trimPathSeparatorLeft(tailPath);
    return QString("%1%2%3").arg(trimmedHead)
                            .arg(QDir::separator())
                            .arg(tailPath);
}

std::string PATH::join(const std::string &headPath, const std::string &tailPath)
{
    return join(QString::fromStdString(headPath), QString::fromStdString(tailPath)).toStdString();
}

QString PATH::getDirFromPath(const QFileInfo &fi)
{
    if(fi.isDir())
        return fi.absoluteFilePath();
    else if (fi.isFile())
        return fi.dir().absolutePath();
    else // dann lief wohl was schief....
        throw "PATH::getDirFromPath -> absPath is neither a folder nor a file!";
}
QString PATH::getDirFromPath(const QString &absPath)
{
    return getDirFromPath(QFileInfo(absPath));
}

QString PATH::getDirFromPath(const std::string &absPath)
{
    return getDirFromPath(QString::fromStdString(absPath));
}
