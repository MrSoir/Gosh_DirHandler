#include "direlapseworker.h"

DirElapseWorker::DirElapseWorker(std::vector<FileInfoBD*> dirsToElapse,
                           bool recursive,
                           bool collapse)
    : DirManagerWorker(nullptr),
      m_dirsToElapse(dirsToElapse),
      m_recursive(recursive),
      m_collapse(collapse),
      m_useSubThreads(!collapse),
      m_runningThreads(0)
{
    connectSignals();
    STATIC_FUNCTIONS::removeSubDirsIfParentDirIsInContainer(m_dirsToElapse);
}

DirElapseWorker::DirElapseWorker(FileInfoBD *dirToElapse,
                           bool recursive,
                           bool collapse)
    : DirManagerWorker(nullptr),
      m_dirsToElapse(std::vector<FileInfoBD*>()),
      m_recursive(recursive),
      m_collapse(collapse),
      m_useSubThreads(!collapse),
      m_runningThreads(0)
{
    m_dirsToElapse.push_back(dirToElapse);
    connectSignals();
}

DirElapseWorker::~DirElapseWorker()
{
    qDebug() << "~DirElapseWorker";
}

bool DirElapseWorker::blockOtherThreads() const
{
    return false;
}

bool DirElapseWorker::revalidateDirStructureAfterWorkerHasFinished() const
{
    return true;
}

void DirElapseWorker::run()
{
    qDebug() << "DirElapseWorker::run";
    for(auto* dir: m_dirsToElapse)
    {
        if(m_collapse)
        {
            collapseHelper(dir);
        }else{
            ++m_runningThreads;

            QThread* thread = new QThread();

            DirElapseWorkerHelper* helper = new DirElapseWorkerHelper(dir, m_recursive);
            helper->moveToThread(thread);
            connect(helper, &DirElapseWorkerHelper::finished, this, &DirElapseWorker::workerFinished, Qt::QueuedConnection);
            connect(this, &DirElapseWorker::killHelpers, helper, &DirElapseWorkerHelper::cancel, Qt::DirectConnection);

            connect(helper, &DirElapseWorkerHelper::finished, thread, &QThread::quit);
            connect(thread, &QThread::finished, thread, &QThread::deleteLater);
            connect(thread, &QThread::started, helper, &DirElapseWorkerHelper::run);

            thread->start();
        }
    }
    if( !m_useSubThreads )
    {
        emit finished(revalidateDirStructureAfterWorkerHasFinished());
    }
}

void DirElapseWorker::workerFinished(FileInfoBD *dir)
{
    Q_UNUSED(dir)
    --m_runningThreads;
//    dir->moveToThread(this->thread());

    if(m_runningThreads <= 0)
    {
        emit finished(revalidateDirStructureAfterWorkerHasFinished());
    }
}

void DirElapseWorker::connectSignals()
{
    connect(this, &DirElapseWorker::cancelled, this, &DirElapseWorker::killHelpers, Qt::QueuedConnection);
    connect(this, &DirElapseWorker::runTask, this, &DirElapseWorker::run);
}

void DirElapseWorker::collapseHelper(FileInfoBD *dir)
{
    dir->collapse();

    if(m_cancelled)
        return;

    if(m_recursive)
    {
        for(auto* sub_dir: dir->getSubFolders())
        {
            if(m_cancelled)
                return;

            collapseHelper(sub_dir);
        }
    }
}


//----------------------------------------------------------------------------------------



DirElapseWorkerHelper::DirElapseWorkerHelper(FileInfoBD* dir, bool recursive)
    : QObject(nullptr),
      m_dir(dir),
      m_recursive(recursive),
      m_cancelled(false)
{
    connect(this, &DirElapseWorkerHelper::finished, this, &DirElapseWorkerHelper::deleteLater);
}

DirElapseWorkerHelper::~DirElapseWorkerHelper()
{
    qDebug() << "~DirElapseWorkerHelper";
}

void DirElapseWorkerHelper::run()
{
    qDebug() << "DirElapseWorkerHelper::run";
    elapseHelper(m_dir);
    emit finished(m_dir);
}

void DirElapseWorkerHelper::cancel()
{
    m_cancelled = true;
}

void DirElapseWorkerHelper::elapseHelper(FileInfoBD* dir)
{
    if(m_cancelled)
        return;

    dir->elapse();

    if(m_cancelled)
        return;

    if(m_recursive)
    {
        for(auto* sub_fold: dir->getSubFolders())
        {
            elapseHelper(sub_fold);
            if(m_cancelled)
                break;
        }
    }
}
