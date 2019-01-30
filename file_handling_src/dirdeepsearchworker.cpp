#include "dirdeepsearchworker.h"


DirDeepSearchWorker::DirDeepSearchWorker(std::string keyword, FileInfoBD *root_dir, bool includeHiddenFiles)
    : DirManagerWorker(nullptr),
      m_keyword(keyword),
      m_root_dir(root_dir),
      m_includeHiddenFiles(includeHiddenFiles),
      m_matches(std::vector<DeepSearchResult>()),
      m_runningThreads(0)
{
    connectSignals();
}

DirDeepSearchWorker::~DirDeepSearchWorker()
{
    qDebug() << "~DirDeepSearchWorker";
}

bool DirDeepSearchWorker::blockOtherThreads() const
{
    return true;
}

bool DirDeepSearchWorker::revalidateDirStructureAfterWorkerHasFinished() const
{
    return true;
}

void DirDeepSearchWorker::run()
{
    search(m_root_dir);
}

void DirDeepSearchWorker::workerFinished(std::vector<DeepSearchResult> results, FileInfoBD *dir)
{
    Q_UNUSED(dir)
//    if(dir)
//        dir->moveToThread(this->thread());

    for(const auto& match: results)
        m_matches.push_back(match);

    --m_runningThreads;

    if(m_runningThreads <= 0)
    {
        elapseMatchingFolders();
    }
}

void DirDeepSearchWorker::connectSignals()
{
    connect(this, &DirDeepSearchWorker::cancelled, this, &DirDeepSearchWorker::killHelpers, Qt::QueuedConnection);
    connect(this, &DirDeepSearchWorker::runTask, this, &DirDeepSearchWorker::run);
}

void DirDeepSearchWorker::search(FileInfoBD* dir)
{
    std::string root_file_name = dir->fileName();
    if(StringOps::inStringIgnoreCase(root_file_name, m_keyword))
    {
        DeepSearchResult match;
        match.isDir = true;
        match.absPath = dir->absPath();
        match.closestParentDir = dir;
        match.containingFiBDExists = true;
        m_matches.push_back(match);
    }

    for(auto* sub_dir: dir->getSubFolders())
    {
        ++m_runningThreads;

        QThread* thread = new QThread();

        DirDeepSearchHelper* helper = new DirDeepSearchHelper(m_keyword, sub_dir, m_includeHiddenFiles);
        helper->moveToThread(thread);
        connect(helper, &DirDeepSearchHelper::finished, this, &DirDeepSearchWorker::workerFinished, Qt::QueuedConnection);
        connect(this, &DirDeepSearchWorker::killHelpers, helper, &DirDeepSearchHelper::cancel, Qt::DirectConnection);

        connect(helper, &DirDeepSearchHelper::finished, thread, &QThread::quit);
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        connect(thread, &QThread::started, helper, &DirDeepSearchHelper::run);

        thread->start();
    }
}

void DirDeepSearchWorker::elapseMatchingFolders()
{
    for(const auto& match: m_matches)
    {
        if(!match.containingFiBDExists)
        {
            std::string match_dir = PATH::getDirFromPath(match.absPath).toStdString();
            elapseMatchingFolder_hlpr(match.absPath,
                                      match_dir,
                                      match.isDir,
                                      match.closestParentDir);
        }
    }

    std::vector<std::string> matching_paths;
    for(const auto& match: m_matches)
    {
        matching_paths.push_back(match.absPath);
    }
    emit deepSearchFinished(matching_paths, m_keyword);
    emit finished(revalidateDirStructureAfterWorkerHasFinished());
}

bool DirDeepSearchWorker::elapseMatchingFolder_hlpr(const std::string& match_absPath,
                                                    const std::string& match_dir_path,
                                                    bool isDir,
                                                    FileInfoBD* dir)
{
    const std::string dir_abs_path = dir->absPath();
    if(isDir && dir_abs_path == match_absPath)
        return true;

    if( !dir->isElapsed() )
    {
        dir->elapse();
    }

    if( !isDir && match_dir_path == dir_abs_path )
    {
        // jetzt ist klar, dass das match ein file ist und dass dieses file im aktuellen FileInfoBD*-dir sein muss
        // -> es muss nichts mehr elapsed werden -> es kann returned werden
        return true;
    }

    for(auto* sub_dir: dir->getSubFolders())
    {
        std::string sub_dir_path = sub_dir->absPath();
        if(STATIC_FUNCTIONS::isSubDirectory(match_absPath, sub_dir_path))
        {
            bool match_found = elapseMatchingFolder_hlpr(match_absPath, match_dir_path, isDir, sub_dir);
            if(match_found)
                return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------------------

DirDeepSearchHelper::DirDeepSearchHelper(std::string keyword, FileInfoBD *dir, bool includeHiddenFiles)
    : QObject(nullptr),
      m_keyword(keyword),
      m_dir(dir),
      m_includeHiddenFiles(includeHiddenFiles),
      m_cancelled(false)
{
}

DirDeepSearchHelper::~DirDeepSearchHelper()
{
    qDebug() << "~DirDeepSearchHelper";
}

void DirDeepSearchHelper::run()
{
    search(m_dir);
    emit finished(m_matches,
                  m_dir);
    this->deleteLater();
}

void DirDeepSearchHelper::cancel()
{
    m_cancelled = true;
}

void DirDeepSearchHelper::search(FileInfoBD *dir)
{
    if(m_cancelled)
        return;

    if(dir->isLoaded())
    {
        std::string root_file_name = dir->fileName();
        if(StringOps::inStringIgnoreCase(root_file_name, m_keyword))
        {
            DeepSearchResult match;
            match.isDir = true;
            match.absPath = dir->absPath();
            match.closestParentDir = dir;
            match.containingFiBDExists = true;

            m_matches.push_back(match);
        }

        const std::vector<std::string>& filePaths = dir->getSortedFiles();
        const std::vector<std::string>& fileNames = dir->getSortedFileNames();
        for(unsigned long i=0; i < filePaths.size() && i < fileNames.size(); ++i)
        {
            const auto& filePath = filePaths[i];
            const auto& fileName = fileNames[i];

            if(StringOps::inStringIgnoreCase(fileName, m_keyword))
            {
                DeepSearchResult match;
                match.isDir = false;
                match.absPath = filePath;
                match.closestParentDir = dir;
                match.containingFiBDExists = true;

                m_matches.push_back(match);
            }
        }

        for(auto* sub_dir: dir->getSubFolders())
        {
            search(sub_dir);
        }
    }else{
        search(QString::fromStdString(dir->absPath()), dir);
    }

}

void DirDeepSearchHelper::search(const QString& dir_path, FileInfoBD* closestParentDir)
{
    if(m_cancelled)
        return;

    std::string dir_name = QFileInfo(dir_path).fileName().toStdString();
    if(StringOps::inStringIgnoreCase(dir_name, m_keyword))
    {
        DeepSearchResult match;
        match.isDir = true;
        match.absPath = dir_path.toStdString();
        match.closestParentDir = closestParentDir;
        match.containingFiBDExists = false;

        m_matches.push_back(match);
    }

    QFileInfoList files = ListFiles::getFilesInDirectory(QDir(dir_path), m_includeHiddenFiles);
    for(const auto& file: files)
    {
        if(StringOps::inStringIgnoreCase(file.fileName().toStdString(), m_keyword))
        {
            DeepSearchResult match;
            match.isDir = false;
            match.absPath = file.absoluteFilePath().toStdString();
            match.closestParentDir = closestParentDir;
            match.containingFiBDExists = false;

            m_matches.push_back(match);
        }
    }

    QFileInfoList dirs = ListFiles::getFoldersInDirectory(QDir(dir_path), m_includeHiddenFiles);
    for(const auto& dir: dirs)
    {
        search(dir.absoluteFilePath(), closestParentDir);
    }
}
