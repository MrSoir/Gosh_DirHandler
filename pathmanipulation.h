#ifndef PATHMANIPULATION_H
#define PATHMANIPULATION_H

#include <QString>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDir>

namespace PATH
{
    QString trimPathSeparatorLeft(const QString& path);
    QString trimPathSeparatorRight(const QString& path);
    QString trimPathSeparator(const QString& path);

    //-----------------------------------------------------------------------

    QString getBasePath(const QString& path);
    QString getBasePath(const QFileInfo& fi);
    QString getBasePath(const std::string& path);

    //-----------------------------------------------------------------------

    QString getRelPathFragment(const QString& absPath, const QString& basePath);
    QString getRelPathFragment(const QFileInfo& fi, const QString& basePath);

    //-----------------------------------------------------------------------

    QString genTargetPath_hlpr(const QString& relPathFragment, const QString& tarDirPath);

    //-----------------------------------------------------------------------

    QString genTargetPath(const QString& absPath, const QString& tarPath);
    QString genTargetPath(const QString& absPath, const QString& basePath, const QString& tarPath);

    QString genTargetPath(const std::string& absPath, const std::string& tarPath);
    QString genTargetPath(const std::string& absPath, const std::string& basePath, const std::string& tarPath);

    //-----------------------------------------------------------------------

    QString getFileNameWithoutExtension(const QString& absPath);

    //-----------------------------------------------------------------------

    QString     join(const QString&     headPath, const QString&     tailPath);
    std::string join(const std::string& headPath, const std::string& tailPath);

    //-----------------------------------------------------------------------

    // getDirFromPath -> wenn absPath ein dir  ist, liefert es den absPath so wieder zurueck
    //                -> wenn absPath ein file ist, liefert es den folder zurueck, in dem das file liegt
    QString getDirFromPath(const QFileInfo& fi);
    QString getDirFromPath(const QString& absPath);
    QString getDirFromPath(const std::string& absPath);

}

#endif // PATHMANIPULATION_H
