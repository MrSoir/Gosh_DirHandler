#include "dirmanager.h"

FiBD_Data::FiBD_Data()
    : abs_path(""),
      folders(std::vector<std::string>()),
      files(std::vector<std::string>()),
      isElapsed(true),
      isLoaded(true)
{
}

FiBD_Data::FiBD_Data(const FiBD_Data &copy)
    : abs_path(copy.abs_path),
      folders(copy.folders),
      files(copy.files),
      isElapsed(copy.isElapsed),
      isLoaded(copy.isLoaded)

{
}

FiBD_Data::~FiBD_Data()
{
}

//----------------------------------------------------------------------

FiBD::FiBD()
    : abs_path(""),
      folders(std::vector<FiBD>()),
      files(std::vector<std::string>()),
      folder_paths(std::vector<std::string>()),
      isElapsed(true),
      isLoaded(true)
{
}

FiBD::FiBD(const FiBD &fiBD)
    : abs_path(fiBD.abs_path),
      folders(fiBD.folders),
      files(fiBD.files),
      folder_paths(fiBD.folder_paths),
      isElapsed(fiBD.isElapsed),
      isLoaded(fiBD.isLoaded)
{
}

FiBD::~FiBD()
{
}

void FiBD::replaceContents(FiBD& dir)
{
    this->abs_path = dir.abs_path;
    this->files = dir.files;
    this->folders = dir.folders;
    this->folder_paths = dir.folder_paths;
    this->isLoaded = dir.isLoaded;
    this->isElapsed = dir.isElapsed;
}

//----------------------------------------------------------------------

DirManager::DirManager(const std::string& root_path,
                       QObject *parent)
    : QObject(parent),
      m_root(new FileInfoBD(root_path)),
      m_root_path(root_path),
      m_path_to_dir(std::unordered_map<std::string, FileInfoBD*>()),
      m_path_fileName(std::unordered_map<std::string, std::string>()),
      m_queue(new DirManagerQueue()),
      m_watcher(new DirFileSystemWatcher()),
      m_closed(false)
{
    registerCustomClasses();

    createThread();

    connectSignals();

    revalidateDirStructure();
}

DirManager::DirManager(FileInfoBD* root_dir, QObject *parent)
    : QObject(parent),
      m_root(root_dir),
      m_root_path(root_dir->absPath()),
      m_path_to_dir(std::unordered_map<std::string, FileInfoBD*>()),
      m_path_fileName(std::unordered_map<std::string, std::string>()),
      m_queue(new DirManagerQueue()),
      m_watcher(new DirFileSystemWatcher()),
      m_closed(false)
{
    registerCustomClasses();

    createThread();

    connectSignals();

    revalidateDirStructure();
}

DirManager::~DirManager()
{
    qDebug() << "~DirManager";
    if(m_root)
    {
        m_root->close();
    }
}

FiBD DirManager::getTree() const
{
    return genTreeFromRoot();
}

void DirManager::deepSearch(std::string keyword, bool includeHiddenFiles)
{
    if(m_closed)
        return;

    DirDeepSearchWorker* worker = new DirDeepSearchWorker(keyword, m_root, includeHiddenFiles);
    connect(worker, &DirDeepSearchWorker::deepSearchFinished, this, &DirManager::deepSearchFinished, Qt::QueuedConnection);
    emit addWorker(worker);
}

void DirManager::elapse(std::string path)
{
    if(m_closed)
        return;

    elapse_hlpr({path}, false, false);

}

void DirManager::elapse(std::vector<std::string> paths)
{
    if(m_closed)
        return;

    elapse_hlpr(paths, false, false);
}

void DirManager::elapseRec(std::string path)
{
    if(m_closed)
        return;

    elapse_hlpr({path}, true, false);

}

void DirManager::elapseRec(std::vector<std::string> paths)
{
    if(m_closed)
        return;

    elapse_hlpr(paths, true, false);
}

void DirManager::collapse(std::string path)
{
    if(m_closed)
        return;

    elapse_hlpr({path}, false, true);

}

void DirManager::collapse(std::vector<std::string> paths)
{
    if(m_closed)
        return;

    elapse_hlpr(paths, false, true);
}

void DirManager::collapseRec(std::string path)
{
    if(m_closed)
        return;

    elapse_hlpr({path}, true, true);
}

void DirManager::collapseRec(std::vector<std::string> paths)
{
    if(m_closed)
        return;

    elapse_hlpr(paths, true, true);
}

void DirManager::deleteDir(string dir)
{
    if(m_closed)
        return;

    delete_hlpr({dir});
}

void DirManager::deleteDirs(std::vector<string> dirs)
{
    if(m_closed)
        return;

    delete_hlpr(dirs);
}

void DirManager::deleteDir(FiBDDeletor* dir)
{
    if(m_closed)
        return;

    delete_hlpr({dir});
}

void DirManager::deleteDirs(std::vector<FiBDDeletor*> dirs)
{
    if(m_closed)
        return;

    delete_hlpr(dirs);
}

void DirManager::cdUP()
{
    std::string base_dir = PATH::getBasePath(m_root_path).toStdString();
    std::string old_root_path = m_root_path;
    if(base_dir.empty() && base_dir != m_root_path)
    {
        FileInfoBD* new_root_dir = new FileInfoBD(base_dir);
        new_root_dir->elapse();
        new_root_dir->replaceSub_fold(old_root_path, m_root);
        m_root_path = base_dir;
    }
}

void DirManager::revalidateDirStructure()
{
    if(!m_closed)
    {
        clearContainers();
        if(m_root)
        {
            m_root_path = m_root->absPath();
            revalidateDirStructure_hlpr(m_root);
        }

        addDirsToWatcher_helpr();

        emit treeChanged(genTreeFromRoot());
        emit dirStructureRevalidated();
    }
}

void printDir(FileInfoBD* dir, int padding = 0)
{
    QString q_padding("");
    for(int i=0; i < padding; ++i)
    {
        q_padding.append("  ");
    }
    qDebug() << q_padding << QString::fromStdString(dir->absPath());
    for(auto* sub_dir: dir->getSubFolders())
    {
        printDir(sub_dir, padding + 1);
    }
    for(const auto& file_path: dir->getSortedFiles())
    {
        qDebug() << q_padding << "  " << QString::fromStdString(file_path);
    }
}

void DirManager::dirChanged_slot(std::string dir_path)
{
    qDebug() << "DirManager::dirChanged_slot: " << QString::fromStdString(dir_path);
    if(m_path_to_dir.find(dir_path) != m_path_to_dir.end())
    {
        FileInfoBD* dir = m_path_to_dir[dir_path];

        dir->revalidate();

//        printDir(dir);

        FiBD revalidated_dir = genFiBDFromDir(dir);

        emit dirChanged(revalidated_dir);
    }
}

void DirManager::close()
{
    m_closed = true;

    emit closeThread();
}

void DirManager::elapse_hlpr(const std::vector<string>& paths, bool recursive, bool collapse)
{
    if(m_closed)
        return;

    std::vector<FileInfoBD*> dirs;
    for(const auto& path: paths)
    {
        if(m_path_to_dir.find(path) != m_path_to_dir.end())
        {
            dirs.push_back(m_path_to_dir[path]);
        }
    }
    if(dirs.size() > 0)
    {
        DirElapseWorker* worker = new DirElapseWorker(dirs, recursive, collapse);
        emit addWorker(worker);
    }
}

void DirManager::delete_hlpr(std::vector<string> pathsToDelete)
{
    qDebug() << "IN WRONG FUNCTION 1";
    std::vector<FiBDDeletor*> dirs_to_delete;
    for(const auto& dir_path: pathsToDelete)
    {
        if(m_path_to_dir.find(dir_path) != m_path_to_dir.end())
        {
            dirs_to_delete.push_back( new FiBDDeletor(m_path_to_dir[dir_path], true) );
        }
    }
    if(dirs_to_delete.size() > 0)
    {
        delete_hlpr(dirs_to_delete);
    }
}

void DirManager::delete_hlpr(std::vector<FiBDDeletor*> dirsToDelete)
{
    if(dirsToDelete.size()){
        qDebug() << "delete_hlpr: dirsToDelete: " << dirsToDelete.size();
        for(auto* deletor: dirsToDelete)
        {
            qDebug() << "deletor: " << QString::fromStdString(deletor->m_fiBD->absPath()) << "  removeFromPar: " << (deletor->m_removeFromParent ? "true" : "false");
        }
        DirDeleteWorker* worker = new DirDeleteWorker(dirsToDelete);
        emit addWorker(worker);
    }
}

FiBD DirManager::genTreeFromRoot() const
{
    return genFiBDFromDir(m_root);
}
FiBD DirManager::genFiBDFromDir(FileInfoBD* fi) const
{
    FiBD fiBD;
    fiBD.abs_path = fi->absPath();
    fiBD.isLoaded = fi->isLoaded();
    fiBD.isElapsed = fi->isElapsed();
    fiBD.folder_paths = fi->getSortedFolds();
    fiBD.files = fi->getSortedFiles();
    for(auto* sub_fold: fi->getSubFolders())
    {
        fiBD.folders.push_back(genFiBDFromDir(sub_fold));
    }
    return  fiBD;
}



void DirManager::createThread()
{
    QThread* thread = new QThread();
    m_thread = thread;
    this->moveToThread(thread);

    connect(this, &DirManager::closeThread, thread, &QThread::quit, Qt::QueuedConnection);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    thread->start();
}


//---------------------------------------------------------------------------------------------------------------

void DirManager::connectSignals()
{
    connectQueue();
    connectWatcher();
    connectDirs();
}

//---

void DirManager::connectQueue()
{
    connect(this, &DirManager::closeThread, m_queue, &DirManagerQueue::close);
    connect(m_queue, &DirManagerQueue::noMoreWorkersRunning, this, &DirManager::cleanUp);

    connect(this, &DirManager::addWorker, m_queue, &DirManagerQueue::addWorker);
    connect(m_queue, &DirManagerQueue::revalidateDirStructure, this, &DirManager::revalidateDirStructure);
    connect(this, &DirManager::dirStructureRevalidated, m_queue, &DirManagerQueue::dirStructureRevalidated);
}

//---

void DirManager::connectWatcher()
{
    connect(m_watcher, &DirFileSystemWatcher::dirChanged, this, &DirManager::dirChanged_slot);
    connect(this, &DirManager::addDirToWatcher, m_watcher, &DirFileSystemWatcher::addDir);
    connect(this, &DirManager::addDirsToWatcher, m_watcher, &DirFileSystemWatcher::addDirs);
    connect(this, &DirManager::clearWatcherPaths, m_watcher, &DirFileSystemWatcher::clearDirs);
}

//---

void DirManager::connectDirs()
{
    connect(FIBD_COLLECTOR, &FiBD_Collector::fiBD_created,   this, &DirManager::connectDir,    Qt::DirectConnection);
    connect(FIBD_COLLECTOR, &FiBD_Collector::fIBD_destroyed, this, &DirManager::disconnectDir, Qt::DirectConnection);
}

void DirManager::connectDir(FileInfoBD* dir)
{
    QObject::connect(dir, SIGNAL(requestClosing(std::vector<FiBDDeletor*>)),
                     this,  SLOT(deleteDirs(std::vector<FiBDDeletor*>)));
}

void DirManager::disconnectDir(FileInfoBD* dir)
{
    QObject::disconnect(dir, SIGNAL(requestClosing(std::vector<FiBDDeletor*>)),
                        this,  SLOT(deleteDirs(std::vector<FiBDDeletor*>)));
}

//---------------------------------------------------------------------------------------------------------------


void DirManager::revalidateDirStructure_hlpr(FileInfoBD* fiBD)
{
//    fiBD->moveToThread(m_thread);

    std::string absPath = fiBD->absPath();
    std::string fileName = fiBD->getFileInfo().fileName().toStdString();
    m_path_to_dir[absPath] = fiBD;
    m_path_fileName[absPath] = fileName;
    for(auto* sub_dir: fiBD->getSubFolders())
    {
        if(m_closed)
            return;
        revalidateDirStructure_hlpr(sub_dir);
    }
}

void DirManager::clearContainers()
{
    m_path_to_dir.clear();
    m_path_fileName.clear();
}

void DirManager::registerCustomClasses()
{
    qRegisterMetaType<FiBD_Data>();
    qRegisterMetaType<FiBD>();
    qRegisterMetaType<std::vector<FiBD_Data>>();
    qRegisterMetaType<std::vector<FiBD>>();
}

void DirManager::addDirsToWatcher_helpr()
{
    std::vector<std::string> dir_paths;
    for(const auto& it: m_path_to_dir)
    {
        dir_paths.push_back(it.first);
    }
    emit clearWatcherPaths();
    emit addDirsToWatcher(dir_paths);
}

void DirManager::cleanUp()
{
    clearContainers();

    delete m_root;
    m_root = nullptr;

    m_queue = nullptr;
    // m_queue ruft auf sich selbst nach dem senden DirManagerQueue::noMoreWorkersRunning deleteLater auf! muss hier nicht getan //werden

    this->deleteLater();
}


