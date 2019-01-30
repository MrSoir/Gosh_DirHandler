#ifndef DIRMANAGER_H
#define DIRMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QDebug>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "fileinfobd.h"
#include "pathmanipulation.h"
#include "dirmanagerqueue.h"
#include "direlapseworker.h"
#include "dirdeleteworker.h"
#include "dirdeepsearchworker.h"
#include "dirfilesystemwatcher.h"


class FiBD_Data{
public:
    FiBD_Data();
    FiBD_Data(const FiBD_Data& copy);
    virtual ~FiBD_Data();

    std::string abs_path;
    std::vector<std::string> folders;
    std::vector<std::string> files;
    bool isElapsed;
    bool isLoaded;
};

class FiBD{
public:
    FiBD();
    FiBD(const FiBD& fiBD);
    virtual ~FiBD();

    void replaceContents(FiBD& dir);

    std::string abs_path;
    std::vector<FiBD> folders;
    std::vector<std::string> files;
    std::vector<std::string> folder_paths;
    bool isElapsed;
    bool isLoaded;
};

Q_DECLARE_METATYPE(FiBD_Data);
Q_DECLARE_METATYPE(FiBD);
Q_DECLARE_METATYPE(std::vector<FiBD_Data>);
Q_DECLARE_METATYPE(std::vector<FiBD>);

class DirManager : public QObject
{
    Q_OBJECT
public:
    explicit DirManager(const std::string& root_path,
                        QObject *parent = nullptr);
    explicit DirManager(FileInfoBD* root_dir,
                        QObject *parent = nullptr);
    virtual ~DirManager() override;

    FiBD getTree() const;

signals:
    void dirChanged(FiBD changedDir);
    void treeChanged(FiBD entireTree);

    void closeThread();

    void addWorker(DirManagerWorker*);

    void dirStructureRevalidated(); // for DirManagerQueue

    void deepSearchFinished(std::vector<std::string> matchingPaths, std::string keyword);

    void addDirToWatcher(std::string dir_path);
    void addDirsToWatcher(std::vector<std::string> dir_paths);
    void clearWatcherPaths();
public slots:
    void deepSearch(std::string keyword,
                    bool includeHiddenFiles);

    void elapse(std::string path);
    void elapse(std::vector<std::string> paths);
    void elapseRec(std::string path);
    void elapseRec(std::vector<std::string> paths);

    void collapse(std::string path);
    void collapse(std::vector<std::string> paths);
    void collapseRec(std::string path);
    void collapseRec(std::vector<std::string> paths);

    void deleteDir(std::string dir);
    void deleteDirs(std::vector<std::string> dirs);
    void deleteDir(FiBDDeletor* dir);
    void deleteDirs(std::vector<FiBDDeletor*> dirs);

    void cdUP();

    void revalidateDirStructure(); // from DirManagerQueue

    void dirChanged_slot(std::string dir_path);

    void close();
private:
    void elapse_hlpr(const std::vector<std::string>& paths, bool recursive, bool collapse);
    void delete_hlpr(std::vector<std::string> pathsToDelete);
    void delete_hlpr(std::vector<FiBDDeletor*> dirsToDelete);

    FiBD genTreeFromRoot() const;
    FiBD genFiBDFromDir(FileInfoBD *fi) const;

    void createThread();

    void connectSignals();
    void connectQueue();
    void connectWatcher();
    void connectDirs();

    void connectDir(FileInfoBD* dir);
    void disconnectDir(FileInfoBD* dir);
//    void connectDirs_hlpr(FileInfoBD* dir);
//    void disconnectDirs_hlpr(FileInfoBD* dir);

    void revalidateDirStructure_hlpr(FileInfoBD* fiBD);
    void clearContainers();

    void registerCustomClasses();

    void addDirsToWatcher_helpr();

    void cleanUp();

//-----------------------------------------------------------------------------------------

    FileInfoBD* m_root;
    std::string m_root_path;
    std::unordered_map<std::string, FileInfoBD*> m_path_to_dir;
    std::unordered_map<std::string, std::string> m_path_fileName;

    DirManagerQueue* m_queue;

    DirFileSystemWatcher* m_watcher;

    bool m_closed;

    QThread* m_thread;
};

#endif // DIRMANAGER_H
