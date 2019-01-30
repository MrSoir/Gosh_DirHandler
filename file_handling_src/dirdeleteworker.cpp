#include "dirdeleteworker.h"


DirDeleteWorker::DirDeleteWorker(std::vector<FiBDDeletor*> dirsToDelete)
    : DirManagerWorker(nullptr),
      m_dirsToDelete(dirsToDelete)
{
    connectSignals();
    evalDirsToDelete();
}

DirDeleteWorker::DirDeleteWorker(FiBDDeletor* dirToDelete)
    : DirManagerWorker(nullptr),
      m_dirsToDelete(std::vector<FiBDDeletor*>())
{
    m_dirsToDelete.push_back(dirToDelete);
    connectSignals();
}

DirDeleteWorker::~DirDeleteWorker()
{
    qDebug() << "~DirDeleteWorker";
}

bool DirDeleteWorker::blockOtherThreads() const
{
    return true;
}

bool DirDeleteWorker::revalidateDirStructureAfterWorkerHasFinished() const
{
    return true;
}

void DirDeleteWorker::run()
{
    for(auto& dirToDel: m_dirsToDelete)
    {
        deleteHelper(dirToDel);
    }
    while(m_dirsToDelete.size() > 0)
    {
        auto* dir = m_dirsToDelete[0];
        m_dirsToDelete.erase(m_dirsToDelete.begin());
        delete dir;
    }
    emit finished(revalidateDirStructureAfterWorkerHasFinished());
}

void DirDeleteWorker::connectSignals()
{
    connect(this, &DirDeleteWorker::runTask, this, &DirDeleteWorker::run);
}

void DirDeleteWorker::deleteHelper(FiBDDeletor *dir)
{
    dir->execute_deletion();
}

void DirDeleteWorker::evalDirsToDelete()
{
    std::vector<std::string> paths_to_delete;
    std::unordered_map<std::string, FiBDDeletor*> path_to_deletor;

    for(auto* dir: m_dirsToDelete)
    {
        std::string absPath = dir->m_fiBD->absPath();
        paths_to_delete.push_back(absPath);
        path_to_deletor[absPath] = dir;
    }

    bool invalidPathsDetected = STATIC_FUNCTIONS::removeSubDirsIfParentDirIsInContainer(paths_to_delete);

    if(invalidPathsDetected)
    {
        qDebug() << "DirDeleteWorker::evalDirsToDelete - invalidPathsDetected: " << (invalidPathsDetected ? "true" : "false");

        std::vector<FiBDDeletor*> validDirsToDelete;
        for(const auto& path: paths_to_delete)
        {
            validDirsToDelete.push_back( path_to_deletor[path] );
        }
        path_to_deletor.clear();
        m_dirsToDelete = validDirsToDelete;
    }
}
