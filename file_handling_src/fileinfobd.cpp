#include "fileinfobd.h"


//-------------------------------------------------------------------------------------------

FiBD_Collector* FIBD_COLLECTOR = new FiBD_Collector();

FiBD_Collector::FiBD_Collector()
    : QObject(nullptr)
{
}

void FiBD_Collector::fiBD_created_slot(FileInfoBD* fiBD)
{
    emit fiBD_created(fiBD);
}

void FiBD_Collector::fiBD_destroyed_slot(FileInfoBD* fiBD)
{
    emit fIBD_destroyed(fiBD);
}



//----------------------------------------------------------------


FileInfoBD::FileInfoBD(const std::string& path,
                       FileInfoBD* parentFiBD,
                       bool showHiddenFiles,
                       QObject *parent)
    : QObject(parent),      

      m_isElapsed(false),
      m_alrLoaded(false),

      m_fileInfo(QFileInfo(QString(path.c_str()))),

      m_files(std::unordered_map<std::string, QFileInfo>()),
      m_sub_folds(std::unordered_set<FileInfoBD*>()),
      m_path_to_subFold(std::unordered_map<std::string, FileInfoBD*>()),
      m_hidden_files(std::unordered_set<std::string>()),
      m_hidden_folds(std::unordered_set<std::string>()),
      m_sortedFilePaths_incl_hidden(std::vector<std::string>()),
      m_sortedFileNames_incl_hidden(std::vector<std::string>()),
      m_sortedFoldPaths_incl_hidden(std::vector<std::string>()),
      m_entryPaths_incl_hidden(std::vector<std::string>()),
      m_sortedFilePaths_no_hidden(std::vector<std::string>()),
      m_sortedFileNames_no_hidden(std::vector<std::string>()),
      m_sortedFoldPaths_no_hidden(std::vector<std::string>()),
      m_entryPaths_no_hidden(std::vector<std::string>()),

      m_showHiddenFiles(showHiddenFiles),
      m_disableSignals(false),

      m_parent(parentFiBD),

      m_cancelled_elapsing(false),
      m_cancelled_collapsing(false),
      m_cancelled_hidingHiddenFiles(false),
      m_cancelled_revalidation(false),
      m_cancelled_sorting(false),

      m_order(Order()),
      m_current_ordering(Order())
{
    registerThis();
}

FileInfoBD::FileInfoBD(const QFileInfo& fileInfo,
                    FileInfoBD* parentFiBD,
                    bool showHiddenFiles,
                    QObject *parent)
    : QObject(parent),

      m_isElapsed(false),
      m_alrLoaded(false),

      m_fileInfo(fileInfo),

      m_files(std::unordered_map<std::string, QFileInfo>()),
      m_sub_folds(std::unordered_set<FileInfoBD*>()),
      m_path_to_subFold(std::unordered_map<std::string, FileInfoBD*>()),
      m_hidden_files(std::unordered_set<std::string>()),
      m_hidden_folds(std::unordered_set<std::string>()),
      m_sortedFilePaths_incl_hidden(std::vector<std::string>()),
      m_sortedFileNames_incl_hidden(std::vector<std::string>()),
      m_sortedFoldPaths_incl_hidden(std::vector<std::string>()),
      m_entryPaths_incl_hidden(std::vector<std::string>()),
      m_sortedFilePaths_no_hidden(std::vector<std::string>()),
      m_sortedFileNames_no_hidden(std::vector<std::string>()),
      m_sortedFoldPaths_no_hidden(std::vector<std::string>()),
      m_entryPaths_no_hidden(std::vector<std::string>()),

      m_showHiddenFiles(showHiddenFiles),
      m_disableSignals(false),

      m_parent(parentFiBD),

      m_cancelled_elapsing(false),
      m_cancelled_collapsing(false),
      m_cancelled_hidingHiddenFiles(false),
      m_cancelled_revalidation(false),
      m_cancelled_sorting(false),

      m_order(Order()),
      m_current_ordering(Order())
{
    registerThis();
}

FileInfoBD::FileInfoBD(const FileInfoBD &fi)
    : QObject(fi.parent()),

      m_isElapsed(fi.m_isElapsed),
      m_alrLoaded(fi.m_alrLoaded),

      m_fileInfo(fi.m_fileInfo),

      m_files(fi.m_files),
      m_sub_folds(fi.m_sub_folds),
      m_path_to_subFold(fi.m_path_to_subFold),
      m_hidden_files(fi.m_hidden_files),
      m_hidden_folds(fi.m_hidden_folds),
      m_sortedFilePaths_incl_hidden(fi.m_sortedFilePaths_incl_hidden),
      m_sortedFileNames_incl_hidden(fi.m_sortedFileNames_incl_hidden),
      m_sortedFoldPaths_incl_hidden(fi.m_sortedFoldPaths_incl_hidden),
      m_entryPaths_incl_hidden(fi.m_entryPaths_incl_hidden),
      m_sortedFilePaths_no_hidden(fi.m_sortedFilePaths_no_hidden),
      m_sortedFileNames_no_hidden(fi.m_sortedFileNames_no_hidden),
      m_sortedFoldPaths_no_hidden(fi.m_sortedFoldPaths_no_hidden),
      m_entryPaths_no_hidden(fi.m_entryPaths_no_hidden),

      m_showHiddenFiles(fi.m_showHiddenFiles),
      m_disableSignals(fi.m_disableSignals),

      m_parent(fi.m_parent),

      m_cancelled_elapsing(false),
      m_cancelled_collapsing(false),
      m_cancelled_hidingHiddenFiles(false),
      m_cancelled_revalidation(false),
      m_cancelled_sorting(false),

      m_order(fi.m_order),
      m_current_ordering(fi.m_current_ordering)
{
    registerThis();
}

void FileInfoBD::operator=(const FileInfoBD& fi)
{
    this->m_isElapsed = fi.m_isElapsed;
    this->m_alrLoaded = fi.m_alrLoaded;

    this->m_fileInfo = fi.m_fileInfo;

    this->m_files = fi.m_files;
    this->m_sub_folds = fi.m_sub_folds;
    this->m_sub_fold_paths = fi.m_sub_fold_paths;
    this->m_path_to_subFold = fi.m_path_to_subFold;
    this->m_hidden_files = fi.m_hidden_files;
    this->m_hidden_folds = fi.m_hidden_folds;
    this->m_sortedFilePaths_incl_hidden = fi.m_sortedFilePaths_incl_hidden;
    this->m_sortedFileNames_incl_hidden = fi.m_sortedFileNames_incl_hidden;
    this->m_sortedFoldPaths_incl_hidden = fi.m_sortedFoldPaths_incl_hidden;
    this->m_entryPaths_incl_hidden = fi.m_entryPaths_incl_hidden;
    this->m_sortedFilePaths_no_hidden = fi.m_sortedFilePaths_no_hidden;
    this->m_sortedFileNames_no_hidden = fi.m_sortedFileNames_no_hidden;
    this->m_sortedFoldPaths_no_hidden = fi.m_sortedFoldPaths_no_hidden;
    this->m_entryPaths_no_hidden = fi.m_entryPaths_no_hidden;

    this->m_showHiddenFiles = fi.m_showHiddenFiles;
    this->m_disableSignals = fi.m_disableSignals;

    this->m_parent = fi.m_parent;

    this->m_cancelled_elapsing = fi.m_cancelled_elapsing;
    this->m_cancelled_collapsing = fi.m_cancelled_collapsing;
    this->m_cancelled_hidingHiddenFiles = fi.m_cancelled_hidingHiddenFiles;
    this->m_cancelled_revalidation = fi.m_cancelled_revalidation;
    this->m_cancelled_sorting = fi.m_cancelled_sorting;

    this->m_order = fi.m_order;
    this->m_current_ordering = fi.m_current_ordering;

    registerThis();
}

FileInfoBD::~FileInfoBD()
{
    qDebug() << "~FileInfoBD: " << m_fileInfo.absoluteFilePath();
    unregisterThis();
}

//-----------------------------------------------------

bool FileInfoBD::isElapsed() const
{
    return m_isElapsed;
}
bool FileInfoBD::isLoaded() const
{
    return m_alrLoaded;
}

unsigned long FileInfoBD::getFileCount() const
{
    return m_files.size();
}
unsigned long FileInfoBD::getFolderCount() const
{
    return m_sub_folds.size();
}
unsigned long FileInfoBD::getContentCount() const
{
    return m_sub_folds.size() + m_files.size();
}

unsigned long FileInfoBD::getShownFileCount() const
{
    return m_files.size() - (!m_showHiddenFiles ? m_hidden_files.size() : 0);
}
unsigned long FileInfoBD::getShownFolderCount() const
{
    return m_sub_folds.size() - (!m_showHiddenFiles ? m_hidden_folds.size() : 0);
}
unsigned long FileInfoBD::getShownContentCount() const
{
    return (m_files.size() + m_sub_folds.size()) - (!m_showHiddenFiles ? m_hidden_files.size() + m_hidden_folds.size() : 0);
}

void FileInfoBD::disableSignals(bool disableSignals)
{
    m_disableSignals = disableSignals;
}

const QFileInfo &FileInfoBD::getFileInfo() const
{
    return m_fileInfo;
}

void FileInfoBD::close(bool remFromPrnt)
{
    qDebug() << "FileInfoBD::close - removeFromParent: " << (remFromPrnt ? "true" : "false");

    close_hlpr();
    qDebug() << "FileInfoBD::close - close_hlpr finished";

    if(m_parent && remFromPrnt)
    {
        m_parent->removeSubFolder(this);
        m_parent = nullptr;
    }

    delete this;
    qDebug() << "FileInfoBD::close finished";
}

void FileInfoBD::close_hlpr()
{
    m_disableSignals = true;

    std::vector<FileInfoBD*> sub_folds;//m_sub_folds.begin(), m_sub_folds.end());
    for(auto* sub_fold: m_sub_folds)
    {
        sub_folds.push_back(sub_fold);
    }

    clearContainers(true);

    while(sub_folds.size() > 0)
    {
        FileInfoBD* sub_fold = sub_folds[0];
        if(sub_fold)
        {
            sub_folds.erase(sub_folds.begin());

            sub_fold->close_hlpr();
            delete sub_fold;
        }
    }
}

std::string FileInfoBD::fileName() const
{
    return m_fileInfo.fileName().toStdString();
}
std::string FileInfoBD::absPath() const
{
    return m_fileInfo.absoluteFilePath().toStdString();
}

void FileInfoBD::print() const
{
    qDebug() << m_fileInfo.fileName();
    int i=0;
    for(auto it = m_files.begin(); it != m_files.end(); ++it, ++i)
    {
        qDebug() << "file[" << i << "]: " << QString::fromStdString(it->first);
    }
    for(auto it = m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->print();
    }
}

void FileInfoBD::iterate(std::function<void(FileInfoBD*,
                                            std::string,
                                            std::string,
                                            bool,
                                            bool,
                                            bool,
                                            FileInfoBD*,
                                            int depthId)> f,
                         int initDepthId)
{
    bool isCollapsed = m_parent ? m_parent->m_isElapsed : false;
    FileInfoBD* fncf = this; // fncf = firstNonCollapsedFold
    FileInfoBD* pit = this; // parent_iterator
    while(pit)
    {
        if(!pit || pit->m_isElapsed)
        {
            break;
        }
        fncf = pit;
        pit = pit->m_parent;

    }
    iterate_helper(f, isCollapsed, fncf, initDepthId);
}
void FileInfoBD::iterate_helper(std::function<void(FileInfoBD*,
                                                   std::string,
                                                   std::string,
                                                   bool,
                                                   bool,
                                                   bool,
                                                   FileInfoBD* firstNonCollapsedFold,
                                                   int depthId)> f,
                                bool isCollapsed,
                                FileInfoBD* firstNonCollapsedFold,
                                int depthId)
{
    FileInfoBD* fncf = m_isElapsed ? this : firstNonCollapsedFold;

    bool isHidden = m_fileInfo.isHidden() && !m_showHiddenFiles;

    f(this, absPath(), fileName(), true, isCollapsed, isHidden, fncf, depthId);

    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->iterate_helper(f, isCollapsed || !m_isElapsed, fncf, depthId+1);
    }
    for(auto it: m_files)
    {
        std::string filePath = it.first;
        std::string fileName = it.second.fileName().toStdString();

        bool file_isHidden = ((m_hidden_files.find(filePath) != m_hidden_files.end()) && !m_showHiddenFiles);

        f(this, filePath, fileName, false, isCollapsed, file_isHidden, fncf, depthId+1);
    }
}

void FileInfoBD::iterateOverFolders(std::function<void (const FileInfoBD* const)> f) const
{
    f(this);
    for(const auto* subFolder: m_sub_folds)
        subFolder->iterateOverFolders(f);
}

const std::vector<std::string>& FileInfoBD::getSortedFiles() const
{
    return m_showHiddenFiles ? m_sortedFilePaths_incl_hidden : m_sortedFilePaths_no_hidden;
}

const std::vector<std::string>& FileInfoBD::getSortedFileNames() const
{
    return m_showHiddenFiles ? m_sortedFileNames_incl_hidden : m_sortedFileNames_no_hidden;
}

const std::vector<std::string>& FileInfoBD::getSortedFolds() const
{
    return m_showHiddenFiles ? m_sortedFoldPaths_incl_hidden : m_sortedFoldPaths_no_hidden;
}

const std::vector<std::string>& FileInfoBD::getSortedEntries() const
{
    return m_showHiddenFiles ? m_entryPaths_incl_hidden : m_entryPaths_no_hidden;
}

std::unordered_set<FileInfoBD*>& FileInfoBD::getSubFolders()
{
    return m_sub_folds;
}


//-----------------------------------------------------

// elapsing-functions:
void FileInfoBD::elapse()
{
    resetCancelFlags();

    elapse_hlpr();
    emit elapsingFinished();
}
void FileInfoBD::elapse_rec()
{
    resetCancelFlags();

    elapse_hlpr();

    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        if(m_cancelled_elapsing)
            break;

        (*it)->elapse_rec();
    }
    emit elapsingFinished();
}
void FileInfoBD::elapse_hlpr()
{
    m_isElapsed = true;
    doElapsing();
    checkIfSortingIsStillValid();
}

void FileInfoBD::cancelElapsing()
{
    m_cancelled_elapsing = true;
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->cancelElapsing();
    }
}

// collapsing-functions:
void FileInfoBD::collapse()
{
    resetCancelFlags();

    collapse_hlpr();
    emit collapsingFinished();
}
void FileInfoBD::collapse_rec()
{
    resetCancelFlags();

    collapse_hlpr();

    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        if(m_cancelled_collapsing)
            break;

        (*it)->collapse_rec();
    }
    emit collapsingFinished();
}
void FileInfoBD::collapse_hlpr()
{
    m_isElapsed = false;
    doElapsing();
}

void FileInfoBD::cancelCollapsing()
{
    m_cancelled_collapsing = true;
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->cancelCollapsing();
    }
}

// show-hide-hiddenFiles-functions:
void FileInfoBD::showHiddenFiles()
{
    resetCancelFlags();

    if( !m_showHiddenFiles )
    {
        showHiddenFiles_hlpr();
    }
    emit showingHiddenFilesFinished();
}
void FileInfoBD::hideHiddenFiles()
{
    resetCancelFlags();

    if(m_showHiddenFiles)
    {
        hideHiddenFiles_hlpr();
    }
    emit hidingHiddenFilesFinished();
}
void FileInfoBD::showHiddenFiles_rec()
{
    resetCancelFlags();

    showHiddenFiles_hlpr();

    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        if(m_cancelled_hidingHiddenFiles)
            break;

        (*it)->showHiddenFiles_rec();
    }
    emit showingHiddenFilesFinished();
}
void FileInfoBD::hideHiddenFiles_rec()
{
    resetCancelFlags();

    hideHiddenFiles_hlpr();
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        if(m_cancelled_hidingHiddenFiles)
            break;

        (*it)->hideHiddenFiles_rec();
    }
    emit hidingHiddenFilesFinished();
}
void FileInfoBD::showHiddenFiles_hlpr()
{
    m_showHiddenFiles = true;
}
void FileInfoBD::hideHiddenFiles_hlpr()
{
    m_showHiddenFiles = false;
}

void FileInfoBD::cancelHidingHiddenFiles()
{
    m_cancelled_hidingHiddenFiles = true;
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->cancelHidingHiddenFiles();
    }
}

void FileInfoBD::revalidate()
{
    m_cancelled_revalidation = false;

    revalFolderContent();
    emit revalidationFinished();
}

void FileInfoBD::cancel_revalidation()
{
    m_cancelled_revalidation = true;
}


void FileInfoBD::sortFolder(Order order)
{
    resetCancelFlags();
    sortFolder_hlpr(order);
    emit sortingFinished();
}
void FileInfoBD::sortFolder_rec(Order order)
{
    resetCancelFlags();
    sortFolder_hlpr_rec(order);
    emit sortingFinished();
}
void FileInfoBD::cancelSorting()
{
    m_cancelled_sorting = true;
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->cancelSorting();
    }
}

void FileInfoBD::replaceSub_fold(std::string subFoldPath, FileInfoBD* subFoldToRepl)
{
    for(auto* sub_fold: m_sub_folds)
    {
        if(sub_fold && sub_fold->absPath() == subFoldPath)
        {
            FileInfoBD* old_sub_fold = sub_fold;
            *old_sub_fold = *sub_fold;

            *sub_fold = *subFoldToRepl;
            std::string sub_fold_path = sub_fold->absPath();
            m_path_to_subFold[sub_fold_path] = subFoldToRepl;
            subFoldToRepl->m_parent = this;

            old_sub_fold->close_hlpr();
            delete old_sub_fold;
        }
    }
}
void FileInfoBD::sortFolder_hlpr(Order order)
{
    m_order = order;
    if(m_cancelled_sorting)
        return;
    doSorting();
}
void FileInfoBD::sortFolder_hlpr_rec(Order order)
{
    sortFolder_hlpr(order);
    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end() && !m_cancelled_sorting; ++it)
    {
        (*it)->sortFolder_hlpr_rec(order);
    }
}
void FileInfoBD::doSorting()
{
    m_sortedFilePaths_incl_hidden.clear();
    m_sortedFileNames_incl_hidden.clear();
    m_sortedFoldPaths_incl_hidden.clear();
    m_entryPaths_incl_hidden.clear();
    m_sortedFilePaths_no_hidden.clear();
    m_sortedFileNames_no_hidden.clear();
    m_sortedFoldPaths_no_hidden.clear();
    m_entryPaths_no_hidden.clear();

    m_current_ordering = m_order;

//    1. sort folders:
    std::vector<FileInfoBD*> subFolds(m_sub_folds.begin(), m_sub_folds.end());

    if(m_order.ordered_by == ORDERED_BY::NAME)
    {
        auto sortFunc = genSortingFunction<FileInfoBD*, std::string>([](FileInfoBD* fi){return StringOps::toLower(fi->fileName());});
        std::sort(subFolds.begin(), subFolds.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::SIZE)
    {
        auto sortFunc = genSortingFunction<FileInfoBD*, qint64>([](FileInfoBD* fi){return fi->m_fileInfo.size();});
        std::sort(subFolds.begin(), subFolds.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::TYPE)
    {
        auto sortFunc = genSortingFunction<FileInfoBD*, std::string>([](FileInfoBD* fi){return StringOps::toLower(fi->m_fileInfo.completeSuffix().toStdString());});
        std::sort(subFolds.begin(), subFolds.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::MOD_DATE)
    {
        auto sortFunc = genSortingFunction<FileInfoBD*, qint64>([](FileInfoBD* fi){return fi->m_fileInfo.lastModified().toMSecsSinceEpoch();});
        std::sort(subFolds.begin(), subFolds.end(), sortFunc);
    }

    m_sortedFoldPaths_incl_hidden.reserve(subFolds.size());
    m_sortedFoldPaths_no_hidden.reserve(subFolds.size()); // nicht schaedlich, wenn mehr reserviert als spater tats. belegt wird -> std::vector::size() gibt dann lediglich die tats. anzahl elemente an, nicht die anzahl, die in std::vector::reserve gesetzt wurde!
    if(m_order.reversedOrdered)
    {
        for(std::vector<FileInfoBD*>::reverse_iterator it = subFolds.rbegin(); it != subFolds.rend(); ++it)
        {
            std::string path = (*it)->absPath();
            m_sortedFoldPaths_incl_hidden.push_back(path);
            if(m_hidden_folds.find(path) == m_hidden_folds.end())
                m_sortedFoldPaths_no_hidden.push_back(path);
        }
    }else{
        for(auto* fold: subFolds){
            std::string path = fold->absPath();
            m_sortedFoldPaths_incl_hidden.push_back(path);
            if(m_hidden_folds.find(path) == m_hidden_folds.end())
                m_sortedFoldPaths_no_hidden.push_back(path);
        }
    }

//---------------------------------------------------------

//    2. sort files:
    std::vector<QFileInfo> files;
    files.reserve(m_files.size());
    // nur die tatsaechlich nicht "hidden" files fuers sorting beruecksichtigen:
    for(const auto& it: m_files)
    {
        files.push_back( it.second );
    }

    if(m_order.ordered_by == ORDERED_BY::NAME)
    {
        auto sortFunc = genSortingFunction<const QFileInfo&, std::string>([](const QFileInfo& fi){return StringOps::toLower(fi.fileName().toStdString());});
        std::sort(files.begin(), files.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::SIZE)
    {
        auto sortFunc = genSortingFunction<const QFileInfo&, qint64>([](const QFileInfo& fi){return fi.size();});
        std::sort(files.begin(), files.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::TYPE)
    {
        auto sortFunc = genSortingFunction<const QFileInfo&, std::string>([](const QFileInfo& fi){return StringOps::toLower(fi.completeSuffix().toStdString());});
        std::sort(files.begin(), files.end(), sortFunc);
    }else if(m_order.ordered_by == ORDERED_BY::MOD_DATE)
    {
        auto sortFunc = genSortingFunction<const QFileInfo&, qint64>([](const QFileInfo& fi){return fi.lastModified().toMSecsSinceEpoch();});
        std::sort(files.begin(), files.end(), sortFunc);
    }

    m_sortedFilePaths_incl_hidden.reserve(files.size());
    m_sortedFilePaths_no_hidden.reserve(files.size());
    if(m_order.reversedOrdered)
    {
        for(std::vector<QFileInfo>::reverse_iterator it = files.rbegin(); it != files.rend(); ++it)
        {
            std::string path = (*it).absoluteFilePath().toStdString();
            std::string fileName = (*it).fileName().toStdString();

            m_sortedFilePaths_incl_hidden.push_back(path);
            m_sortedFileNames_incl_hidden.push_back(fileName);

            if(m_hidden_files.find(path) == m_hidden_files.end())
            {
                m_sortedFilePaths_no_hidden.push_back(path);
                m_sortedFileNames_no_hidden.push_back(fileName);
            }
        }
    }else{
        for(auto& file: files)
        {
            std::string path = file.absoluteFilePath().toStdString();
            std::string fileName = file.fileName().toStdString();

            m_sortedFilePaths_incl_hidden.push_back(path);
            m_sortedFileNames_incl_hidden.push_back(fileName);

            if(m_hidden_files.find(path) == m_hidden_files.end())
            {
                m_sortedFilePaths_no_hidden.push_back(path);
                m_sortedFileNames_no_hidden.push_back(fileName);
            }
        }
    }

//---------------------------------------------------------

//    3. eine zusammenfassung aller entries ( folders + files ):
    m_entryPaths_incl_hidden.reserve(m_sortedFilePaths_incl_hidden.size() + m_sortedFoldPaths_incl_hidden.size());
    for(auto& path: m_sortedFilePaths_incl_hidden)
        m_entryPaths_incl_hidden.push_back(path);
    for(auto& path: m_sortedFoldPaths_incl_hidden)
        m_entryPaths_incl_hidden.push_back(path);

    m_entryPaths_no_hidden.reserve(m_sortedFilePaths_no_hidden.size() + m_sortedFoldPaths_no_hidden.size());
    for(auto& path: m_sortedFilePaths_no_hidden)
        m_entryPaths_no_hidden.push_back(path);
    for(auto& path: m_sortedFoldPaths_no_hidden)
        m_entryPaths_no_hidden.push_back(path);
}

void FileInfoBD::checkIfSortingIsStillValid()
{
    if(m_current_ordering != m_order)
    {
        sortFolder(m_order);
    }
}
template<typename T, typename K>
std::function<bool(T,T)> FileInfoBD::genSortingFunction(std::function<K(T)> characteristicGetter) const
{
    return [=](T t1, T t2){return characteristicGetter(t1) < characteristicGetter(t2);};
}

//-----------------------------------------------------

void FileInfoBD::revalFolderContent()
{
    if(m_cancelled_revalidation)
        return;

    QDir dir(m_fileInfo.absoluteFilePath());

    // einzig m_path_to_subFold darf nicht geloescht werden!
    clearContainers(false);
    m_sub_folds.clear();
    m_sub_fold_paths.clear();

    QFileInfoList filesLst = ListFiles::getFilesInDirectory(dir, true);
    for(int i=0; i < filesLst.size(); ++i)
    {
        m_files.emplace(std::make_pair(filesLst[i].absoluteFilePath().toStdString(), filesLst[i]));
        if(filesLst[i].isHidden())
            m_hidden_files.emplace(filesLst[i].absoluteFilePath().toStdString());
    }


    QFileInfoList foldersLst = ListFiles::getFoldersInDirectory(dir, true);

    std::unordered_set<std::string> newFolder_paths;

    for(int i=0; i < foldersLst.size(); ++i)
    {
        newFolder_paths.emplace( foldersLst[i].absoluteFilePath().toStdString() );
        if(foldersLst[i].isHidden())
            m_hidden_folds.emplace(foldersLst[i].absoluteFilePath().toStdString());
    }

    std::vector<FiBDDeletor*> deletedDirs;

    for(auto it = m_path_to_subFold.begin(); it != m_path_to_subFold.end(); )
    {
        if( newFolder_paths.find( it->first ) == newFolder_paths.end() )
        {
            deletedDirs.push_back( new FiBDDeletor(it->second, false) );
            it = m_path_to_subFold.erase( it );
        }else{
            ++it;
        }
    }
    for(const auto& newFolder_path: newFolder_paths)
    {
        if( m_path_to_subFold.find( newFolder_path ) == m_path_to_subFold.end() )
        {
            m_path_to_subFold.emplace(std::make_pair(newFolder_path, new FileInfoBD(QString::fromStdString(newFolder_path), this)));
        }
    }

    for(auto it = m_path_to_subFold.begin(); it != m_path_to_subFold.end(); ++it)
    {
        m_sub_folds.insert(it->second);
    }
    for(const auto& new_subFoldPath: newFolder_paths)
    {
        m_sub_fold_paths.insert( new_subFoldPath );
    }

    doSorting();

    if(deletedDirs.size() > 0)
    {
        emit requestClosing(deletedDirs);
    }

//    qDebug() << "\nrevalidated: " << QString::fromStdString(absPath());
//    qDebug() << "sorted folders:";
//    for(const auto& path: m_sortedFoldPaths_incl_hidden)
//    {
//        qDebug() << QString::fromStdString(path);
//    }
//    qDebug() << "sorted files:";
//    for(const auto& path: m_sortedFilePaths_incl_hidden)
//    {
//       qDebug() << QString::fromStdString(path);
//    }
//    qDebug() << "\n";

    emit contentHasChanged(m_fileInfo.absoluteFilePath());
}

void FileInfoBD::doElapsing()
{
    if(m_isElapsed && !m_alrLoaded){
        m_alrLoaded = true;

        revalFolderContent();
    }
}

//void FileInfoBD::removeFromParent()
//{
//    m_parent->removeFolder(m_fileInfo);
//}

//bool FileInfoBD::removeFolder(const QFileInfo& fi){
//    for(int i=0; i < m_sub_folds.size(); ++i)
//    {
//        if(m_sub_folds[i]->m_fileInfo == fi)
//        {
//            m_sub_folds[i].reset();
//            m_sub_folds.erase(m_sub_folds.begin() + i);
//            m_sub_fold_names.erase(m_sub_fold_names.begin() + i);
//            return true;
//        }
//    }
//    return false;
//}
//bool FileInfoBD::removeFolder(const QString& foldName){
//    for(int i=0; i < m_sub_folds.size(); ++i)
//    {
//        if(m_sub_folds[i]->m_fileInfo.fileName() == foldName)
//        {
//            m_sub_folds[i].reset();
//            m_sub_folds.erase(m_sub_folds.begin() + i);
//            m_sub_fold_names.erase(m_sub_fold_names.begin() + i);
//            return true;
//        }
//    }
//    return false;resetCancelFlags
//}

bool FileInfoBD::isEmpty() const
{
    return (m_files.size() + m_sub_folds.size()) == 0;
}

void FileInfoBD::clearSubFolderContainers()
{
        m_sub_folds.clear();
        m_path_to_subFold.clear();
        m_sub_fold_paths.clear();
}

void FileInfoBD::clearContainers(bool clearSubFolderContainers)
{
    if(clearSubFolderContainers)
    {
        FileInfoBD::clearSubFolderContainers();
    }

    m_files.clear();  
    m_hidden_files.clear();
    m_hidden_folds.clear();
    m_sortedFilePaths_incl_hidden.clear();
    m_sortedFileNames_incl_hidden.clear();
    m_sortedFoldPaths_incl_hidden.clear();
    m_entryPaths_incl_hidden.clear();
    m_sortedFilePaths_no_hidden.clear();
    m_sortedFileNames_no_hidden.clear();
    m_sortedFoldPaths_no_hidden.clear();
    m_entryPaths_no_hidden.clear();
}

void FileInfoBD::removeSubFolder(FileInfoBD *dirToDelete)
{
    std::string dir_path = dirToDelete->absPath();
    std::string dir_name = dirToDelete->fileName();

    if(m_sub_folds.find(dirToDelete) != m_sub_folds.end())
    {
        m_sub_folds.erase(m_sub_folds.find(dirToDelete));
    }
    if(m_sub_fold_paths.find(dir_path) != m_sub_fold_paths.end())
    {
        m_sub_fold_paths.erase(m_sub_fold_paths.find(dir_path));
    }
    if(m_path_to_subFold.find(dir_name) != m_path_to_subFold.end())
    {
        m_path_to_subFold.erase(dir_name);
    }
    if(m_hidden_folds.find(dir_path) != m_hidden_folds.end())
    {
        m_hidden_folds.erase(m_hidden_folds.find(dir_path));
    }
    doSorting();
}

void FileInfoBD::registerThis()
{
    if(FIBD_COLLECTOR)
    {
        FIBD_COLLECTOR->fiBD_created_slot(this);
    }
}

void FileInfoBD::unregisterThis()
{
    if(FIBD_COLLECTOR)
    {
        FIBD_COLLECTOR->fiBD_destroyed_slot(this);
    }
}


void FileInfoBD::resetCancelFlags()
{
    m_cancelled_elapsing            = false;
    m_cancelled_collapsing          = false;
    m_cancelled_hidingHiddenFiles   = false;
    m_cancelled_revalidation        = false;
    m_cancelled_sorting             = false;

    for(auto it=m_sub_folds.begin(); it != m_sub_folds.end(); ++it)
    {
        (*it)->resetCancelFlags();
    }
}



//-------------------------------------------------------------------------------------------



FiBDDeletor::FiBDDeletor(FileInfoBD* fiBD,
                         bool removeFromParent)
    : QObject(nullptr),
      m_fiBD(fiBD),
      m_removeFromParent(removeFromParent)
{
}

FiBDDeletor::FiBDDeletor(FiBDDeletor& toCopy)
    : QObject(nullptr),
      m_fiBD(toCopy.m_fiBD),
      m_removeFromParent(toCopy.m_removeFromParent)
{
}

void FiBDDeletor::operator=(FiBDDeletor &toCopy)
{
    this->m_fiBD = toCopy.m_fiBD;
    this->m_removeFromParent = toCopy.m_removeFromParent;
}

FiBDDeletor::~FiBDDeletor()
{
    qDebug() << "~FiBDDeletor";
}

void FiBDDeletor::execute_deletion()
{
    if(m_fiBD)
    {
        m_fiBD->close(m_removeFromParent);
        m_fiBD = nullptr;
    }
}
