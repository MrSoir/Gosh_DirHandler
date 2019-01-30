#ifndef LISTFILES_H
#define LISTFILES_H

#include <QFileInfoList>
#include <QDir>
#include <QFileInfo>
#include <QString>

//#include <experimental/filesystem>>
//#include <vector>
//#include <string>
//#include <functional>

#include "stringops.h"

namespace ListFiles
{
    QFileInfoList getEntriesInDirectoryHelper(QDir dir, QDir::Filters flags);

    QFileInfoList getFoldersInDirectory(const QDir&      dir, bool showHiddenFiles = true);
    QFileInfoList getFoldersInDirectory(const QFileInfo& dir, bool showHiddenFiles = true);
    QFileInfoList getFoldersInDirectory(const QString&   dir, bool showHiddenFiles = true);

    QFileInfoList getFilesInDirectory(const QDir&      dir, bool showHiddenFiles = true);
    QFileInfoList getFilesInDirectory(const QFileInfo& dir, bool showHiddenFiles = true);
    QFileInfoList getFilesInDirectory(const QString&   dir, bool showHiddenFiles = true);

    QFileInfoList getEntriesInDirectory(const QDir&      dir, bool showHiddenFiles = true);
    QFileInfoList getEntriesInDirectory(const QFileInfo& dir, bool showHiddenFiles = true);
    QFileInfoList getEntriesInDirectory(const QString&   dir, bool showHiddenFiles = true);

    // functioniert nicht, da gnu g++ experimental/filesystem probleme macht. man muesste
    // '-lstdc++fs als compiler-option hinzufuegen, was aber probelme macht
    // ab gnu g++ 8 sollt filsystem (ohne experimental) verfuegbar sein. alles in allem machts
    // zu viele probleme... daher derzeit noch auf Qt's option zurueckgreifen
//    std::vector<std::string> getEntriesInDirectory(const std::string& path, std::function<bool(const std::string&)> f = [](const std::string& pth){Q_UNUSED(pth);return true;});
//    std::vector<std::string> getFoldersInDirectory(const std::string& path);
//    std::vector<std::string> getFilesInDirectory(const std::string& path);
}

#endif // LISTFILES_H
