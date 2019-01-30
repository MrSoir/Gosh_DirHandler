#ifndef FILEMIMETYPEHANDLER_H
#define FILEMIMETYPEHANDLER_H

#include <QObject>
#include <QChar>
#include <QDir>
#include <QString>
#include <QFileInfo>
#include <QFileInfoList>
#include <QImage>
#include <QImageReader>
#include <QDebug>

#include <string>
#include <vector>
#include <unordered_set>
#include <iostream>
//#include <experimental/filesystem>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <limits.h>
#include <unistd.h>
#include <hash_fun.h>

#include "stringops.h"
#include "listfiles.h"
#include "exec.h"
#include "appinfo.h"

#define MIN_ICON_SIZE 3000

class FileMimeTypeHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileMimeTypeHandler(std::string path,
                                 int min_icon_size = 0,
                                 QObject *parent = nullptr);

    std::vector<AppInfo> getMimeAppInfos() const;
signals:

public slots:
private:
    void evalMatchingApps();

    std::string getExePath() const;

    std::string getResourcesPath() const;
    std::string getMimesInfoDirPath() const;
    std::string getMimesInfoFilePath() const;

    std::vector<std::string> getDesktopFilesPaths() const;
    std::vector<std::string> getIconFilePaths() const;


    std::unordered_set<std::string> findIconInPathsRec(const std::string& icon_name, const std::string& path) const;
    std::string findSufficientlyBigIconInPathRec(const std::string& icon_name, const std::string& path) const;
    std::string getIconPath(const std::string& icon_name) const;

    void createMimeResourceDirIfNotExistent() const;

    void evalSupportedImageFileTypes();
    bool imageFileTypeIsSupported(std::string& compl_suffix) const;

    std::unordered_set<AppInfo> m_matching_apps;
    std::unordered_set<std::string> m_supportedImgeFileTypes;
    std::string m_mime_type;
    std::string m_path;
    int m_min_icon_size;
};

#endif // FILEMIMETYPEHANDLER_H
