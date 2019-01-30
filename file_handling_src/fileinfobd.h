#ifndef FILEINFOBD_H
#define FILEINFOBD_H

#include <QObject>
#include <QVector>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFileSystemWatcher>
#include <QDir>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <memory>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <string>
#include <functional>

#include "listfiles.h"
#include "orderby.h"
#include "stringops.h"

class FileInfoBD;

//---------------------------------------------------------------------------

class FiBDDeletor : public QObject
{
    Q_OBJECT
public:
    FiBDDeletor(FileInfoBD* fiBD = nullptr,
                bool removeFromParent = false);
    FiBDDeletor(FiBDDeletor& toCopy);

    void operator=(FiBDDeletor& toCopy);

    virtual ~FiBDDeletor();


    FileInfoBD* m_fiBD;
    bool m_removeFromParent;

public slots:
    void execute_deletion();
private:
};

//---------------------------------------------------------------------------

class FiBD_Collector : public QObject
{
    Q_OBJECT
public:
    explicit FiBD_Collector();
signals:
    void fiBD_created(FileInfoBD* fiBD);
    void fIBD_destroyed(FileInfoBD* fiBD);
public slots:
    void fiBD_created_slot(FileInfoBD* fiBD);
    void fiBD_destroyed_slot(FileInfoBD* fiBD);
};

extern FiBD_Collector* FIBD_COLLECTOR;

//---------------------------------------------------------------------------

class FileInfoBD : public QObject
{
    Q_OBJECT
public:
    explicit FileInfoBD(const std::string& path,
                        FileInfoBD* parentFiBD = nullptr,
                        bool includeHiddenFiles = false,
                        QObject *parent = nullptr);

    // folgender ist ebenfalls default-constructor (da alle args optional sind):
    explicit FileInfoBD(const QFileInfo& fileInfo = QFileInfo(),
                        FileInfoBD* parentFiBD = nullptr,
                        bool includeHiddenFiles = false,
                        QObject *parentm_focus_match_id = nullptr);
    explicit FileInfoBD(const FileInfoBD& fi);
    void operator=(const FileInfoBD& fi);

    ~FileInfoBD();

    bool isElapsed() const;
    bool isLoaded() const;

    unsigned long getFileCount() const;
    unsigned long getFolderCount() const;
    unsigned long getContentCount() const;
    unsigned long getShownFileCount() const;      // returns number of all files   - hidden-files   if m_showHiddenFiles = false;
    unsigned long getShownFolderCount() const;    // returns number of all folders - hidden-folders if m_showHiddenFiles = false;
    unsigned long getShownContentCount() const;   // returns number of all entries - hidden-entries if m_showHiddenFiles = false;

    void disableSignals(bool disableSignals = true);

//    const QFileInfo getFileAt(int pos) const;
//    int getFileId(const QString fileName) const;

    const QFileInfo& getFileInfo() const;

    std::string fileName() const;
    std::string absPath() const;

    void print() const;

    void iterate(std::function<void(FileInfoBD*,
                                    std::string,
                                    std::string,
                                    bool,
                                    bool,
                                    bool,
                                    FileInfoBD*,
                                    int)> f,
                 int initDepthId = 0);
    void iterate_helper(std::function<void(FileInfoBD*,
                                           std::string,
                                           std::string,
                                           bool,
                                           bool,
                                           bool,
                                           FileInfoBD*,
                                           int)> f,
                        bool isCollapsed,
                        FileInfoBD* firstNonCollapsedFold,
                        int depthId);

    void iterateOverFolders(std::function<void(const FileInfoBD* const)> f) const;

    const std::vector<std::string>& getSortedFiles() const;
    const std::vector<std::string>& getSortedFileNames() const;
    const std::vector<std::string>& getSortedFolds() const;
    const std::vector<std::string>& getSortedEntries() const;
    std::unordered_set<FileInfoBD*>& getSubFolders();

    void close(bool removeFromParent = false);

signals:
    void contentHasChanged(QString path);

    void addDirectoryToWatcher(QString directory);

    void elapsingFinished();
    void collapsingFinished();
    void showingHiddenFilesFinished();
    void hidingHiddenFilesFinished();
    void revalidationFinished();
    void sortingFinished();

    void requestClosing(std::vector<FiBDDeletor*> dirsToDelete);

    void closingFinished();
public slots:
    void elapse();
    void elapse_rec();
    void elapse_hlpr();
    void cancelElapsing();

    void collapse();
    void collapse_rec();
    void collapse_hlpr();
    void cancelCollapsing();

    void showHiddenFiles();
    void hideHiddenFiles();
    void showHiddenFiles_rec();
    void hideHiddenFiles_rec();
    void showHiddenFiles_hlpr();
    void hideHiddenFiles_hlpr();
    void cancelHidingHiddenFiles();

    void revalidate();
    void cancel_revalidation();

    void sortFolder(Order order);
    void sortFolder_rec(Order order);
    void cancelSorting();

    void replaceSub_fold(std::string sub_fold_path, FileInfoBD* sub_fold);
private:
    void sortFolder_hlpr(Order order);
    void sortFolder_hlpr_rec(Order order);
    void doSorting();
    void checkIfSortingIsStillValid();

    template<typename T, typename K>
    std::function<bool(T,T)> genSortingFunction(std::function<K(T)> characteristicGetter) const;

    void revalFolderContent();

    void doElapsing();

//    void removeFromParent();
//    bool removeFolder(const QFileInfo& fi);
//    bool removeFolder(const QString& foldName);

    bool isEmpty() const;

    void clearSubFolderContainers();
    void clearContainers(bool clearSubFolderContainers = false);

    void removeSubFolder(FileInfoBD* dirToDelete);

    void registerThis();
    void unregisterThis();

    void close_hlpr();

//-----------------------------------------------------------------------------------

    bool m_isElapsed;
    bool m_alrLoaded;

    QFileInfo m_fileInfo;

    std::unordered_map<std::string, QFileInfo> m_files;
    std::unordered_set<FileInfoBD*> m_sub_folds;
    std::unordered_set<std::string> m_sub_fold_paths;
    std::unordered_map<std::string, FileInfoBD*> m_path_to_subFold;
    std::unordered_set<std::string> m_hidden_files;
    std::unordered_set<std::string> m_hidden_folds;
    std::vector<std::string> m_sortedFilePaths_incl_hidden; // inclusive hidden files
    std::vector<std::string> m_sortedFileNames_incl_hidden; // inclusive hidden files
    std::vector<std::string> m_sortedFoldPaths_incl_hidden; // inclusive hidden files
    std::vector<std::string> m_entryPaths_incl_hidden; // inclusive hidden entries
    std::vector<std::string> m_sortedFilePaths_no_hidden; // without hidden files
    std::vector<std::string> m_sortedFileNames_no_hidden; // inclusive hidden files
    std::vector<std::string> m_sortedFoldPaths_no_hidden; // without hidden folders
    std::vector<std::string> m_entryPaths_no_hidden; // without hidden entries

//    unsigned long m_filesCount;
//    unsigned long m_subFoldsCount;
//    unsigned long m_contentCount;

    bool m_showHiddenFiles;

    bool m_disableSignals;

    FileInfoBD* m_parent;

    void resetCancelFlags();
    bool m_cancelled_elapsing;
    bool m_cancelled_collapsing;
    bool m_cancelled_hidingHiddenFiles;
    bool m_cancelled_revalidation;
    bool m_cancelled_sorting;

    Order m_order;
    Order m_current_ordering;
};

#endif // FILEINFOBD_H
