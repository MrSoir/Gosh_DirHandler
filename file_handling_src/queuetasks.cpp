#include "queuetasks.h"

QueueTasks::FileInfoBDRevalidator::FileInfoBDRevalidator(FileInfoBD *fibd,
                                                         QObject *parent)
    : QueueTask(1,parent),
      m_fis(std::vector<FileInfoBD*>())
{
    m_fis.push_back(fibd);
    connectSignals();
}

QueueTasks::FileInfoBDRevalidator::FileInfoBDRevalidator(std::vector<FileInfoBD*>& fis,
                                                         QObject *parent)
    : QueueTask(static_cast<int>(fis.size()), parent),
      m_fis(fis)
{
    connectSignals();
}

QueueTasks::FileInfoBDRevalidator::FileInfoBDRevalidator(const QueueTasks::FileInfoBDRevalidator &cpy)
    : QueueTask(cpy.m_jobCounter,
                nullptr),
      m_fis(cpy.m_fis)
{
    connectSignals();
}

QueueTasks::FileInfoBDRevalidator::~FileInfoBDRevalidator()
{
    QueueTask::~QueueTask();
}

void QueueTasks::FileInfoBDRevalidator::run()
{
    emit revalidateFiBDs();
}

void QueueTasks::FileInfoBDRevalidator::connectSignals()
{
    for(auto* fi: m_fis)
    {
        connect(this, &FileInfoBDRevalidator::revalidateFiBDs, fi, &FileInfoBD::revalidate);
        connect(this, &FileInfoBDRevalidator::cancelSGNL, fi, &FileInfoBD::cancel_revalidation);
        connect(fi, &FileInfoBD::revalidationFinished, this, &FileInfoBDRevalidator::onFinished);
    }
}

void QueueTasks::FileInfoBDRevalidator::disconnectSignals()
{
    for(auto* fi: m_fis)
    {
        disconnect(this, &FileInfoBDRevalidator::revalidateFiBDs, fi, &FileInfoBD::revalidate);
        disconnect(this, &FileInfoBDRevalidator::cancelSGNL, fi, &FileInfoBD::cancel_revalidation);
        disconnect(fi, &FileInfoBD::revalidationFinished, this, &FileInfoBDRevalidator::onFinished);
    }
}


//------------------------------------------------------------------------------------



QueueTasks::TaskFolderElapser::TaskFolderElapser(std::vector<FileInfoBD*>& fis,
                                     bool collapse,
                                     bool recursively,
                                     QObject *parent)
    : QueueTask(static_cast<int>(fis.size()),
                parent),
      m_fis(fis),
      m_collapse(collapse),
      m_recursively(recursively)
{
    connectSignals();
}

QueueTasks::TaskFolderElapser::TaskFolderElapser(std::unordered_set<FileInfoBD*>& fis,
                                                 bool collapse,
                                                 bool recursively,
                                                 QObject *parent)
    : QueueTask(static_cast<int>(fis.size()),
                parent),
      m_fis(std::vector<FileInfoBD*>(fis.begin(), fis.end())),
      m_collapse(collapse),
      m_recursively(recursively)
{
    connectSignals();
}
QueueTasks::TaskFolderElapser::TaskFolderElapser(FileInfoBD* fiBD,
                                     bool collapse,
                                     bool recursively,
                                     QObject *parent)
    : QueueTask(1,
                parent),
      m_fis(std::vector<FileInfoBD*>()),
      m_collapse(collapse),
      m_recursively(recursively)
{
    m_fis.push_back(fiBD);
    connectSignals();
}

QueueTasks::TaskFolderElapser::~TaskFolderElapser()
{
}

void QueueTasks::TaskFolderElapser::run()
{
    if(m_collapse)
    {
        if(m_recursively)
        {
            emit elapseFiBD_rec();
        }else{
            emit elapseFiBD();
        }
    }else{
        if(m_recursively)
        {
            emit collapseFiBD_rec();
        }else{
            emit collapseFiBD();
        }
    }
}

void QueueTasks::TaskFolderElapser::connectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        connect(this, &TaskFolderElapser::elapseFiBD,       m_fiBD, &FileInfoBD::elapse);
        connect(this, &TaskFolderElapser::elapseFiBD_rec,   m_fiBD, &FileInfoBD::elapse_rec);
        connect(this, &TaskFolderElapser::collapseFiBD,     m_fiBD, &FileInfoBD::collapse);
        connect(this, &TaskFolderElapser::collapseFiBD_rec, m_fiBD, &FileInfoBD::collapse_rec);
        connect(this, &TaskFolderElapser::cancelSGNL,       m_fiBD, &FileInfoBD::cancelElapsing);
        connect(this, &TaskFolderElapser::cancelSGNL,       m_fiBD, &FileInfoBD::cancelCollapsing);

        connect(m_fiBD, &FileInfoBD::elapsingFinished,   this, &TaskFolderElapser::onFinished);
        connect(m_fiBD, &FileInfoBD::collapsingFinished, this, &TaskFolderElapser::onFinished);
    }
}

void QueueTasks::TaskFolderElapser::disconnectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        disconnect(this, &TaskFolderElapser::elapseFiBD,       m_fiBD, &FileInfoBD::elapse);
        disconnect(this, &TaskFolderElapser::elapseFiBD_rec,   m_fiBD, &FileInfoBD::elapse_rec);
        disconnect(this, &TaskFolderElapser::collapseFiBD,     m_fiBD, &FileInfoBD::collapse);
        disconnect(this, &TaskFolderElapser::collapseFiBD_rec, m_fiBD, &FileInfoBD::collapse_rec);
        disconnect(this, &TaskFolderElapser::cancelSGNL,        m_fiBD, &FileInfoBD::cancelElapsing);
        disconnect(this, &TaskFolderElapser::cancelSGNL,        m_fiBD, &FileInfoBD::cancelCollapsing);

        disconnect(m_fiBD, &FileInfoBD::elapsingFinished,   this, &TaskFolderElapser::onFinished);
        disconnect(m_fiBD, &FileInfoBD::collapsingFinished, this, &TaskFolderElapser::onFinished);
    }
}



//------------------------------------------------------------------------------------


QueueTasks::TaskShowHiddenFiles::TaskShowHiddenFiles(std::vector<FileInfoBD*>& fis,
                                         bool hide,
                                         bool recursive,
                                         QObject *parent)
    : QueueTask(static_cast<int>(fis.size()),
                parent),
      m_fis(fis),
      m_hide(hide),
      m_recursive(recursive)
{
    connectSignals();
}
QueueTasks::TaskShowHiddenFiles::TaskShowHiddenFiles(FileInfoBD *fiBD,
                                         bool hide,
                                         bool recursive,
                                         QObject *parent)
    : QueueTask(1,
                parent),
      m_fis(std::vector<FileInfoBD*>()),
      m_hide(hide),
      m_recursive(recursive)
{
    m_fis.push_back(fiBD);
    connectSignals();
}

QueueTasks::TaskShowHiddenFiles::~TaskShowHiddenFiles()
{
}

void QueueTasks::TaskShowHiddenFiles::run()
{
    if(m_hide)
    {
        if(m_recursive)
            emit hideHiddenFilesFiBD();
        else
            emit hideHiddenFilesFiBD_rec();
    }else{
        if(m_recursive)
            emit showHiddenFilesFiBD();
        else
            emit showHiddenFilesFiBD_rec();
    }
}

void QueueTasks::TaskShowHiddenFiles::connectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        connect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD,        m_fiBD, &FileInfoBD::hideHiddenFiles);
        connect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD_rec,    m_fiBD, &FileInfoBD::hideHiddenFiles_rec);
        connect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD,        m_fiBD, &FileInfoBD::showHiddenFiles);
        connect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD_rec,    m_fiBD, &FileInfoBD::showHiddenFiles_rec);
        connect(this, &TaskShowHiddenFiles::cancelSGNL,          m_fiBD, &FileInfoBD::cancelHidingHiddenFiles);

        connect(m_fiBD, &FileInfoBD::hidingHiddenFilesFinished,   this, &TaskShowHiddenFiles::onFinished);
    }
}

void QueueTasks::TaskShowHiddenFiles::disconnectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        disconnect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD,         m_fiBD, &FileInfoBD::hideHiddenFiles);
        disconnect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD_rec,     m_fiBD, &FileInfoBD::hideHiddenFiles_rec);
        disconnect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD,         m_fiBD, &FileInfoBD::showHiddenFiles);
        disconnect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD_rec,     m_fiBD, &FileInfoBD::showHiddenFiles_rec);
        disconnect(this, &TaskShowHiddenFiles::cancelSGNL,           m_fiBD, &FileInfoBD::cancelHidingHiddenFiles);

        disconnect(m_fiBD, &FileInfoBD::hidingHiddenFilesFinished,   this, &TaskShowHiddenFiles::onFinished);
    }
}

//------------------------------------------------------------------------------------


QueueTasks::TaskFolderSorter::TaskFolderSorter(std::vector<FileInfoBD*>& fis,
                                               Order order,
                                               bool recursive,
                                               QObject *parent)
    : QueueTask(static_cast<int>(fis.size()),
                parent),
      m_fis(fis),
      m_order(order),
      m_recursive(recursive)
{
    connectSignals();
}
QueueTasks::TaskFolderSorter::TaskFolderSorter(FileInfoBD *fiBD,
                                               Order order,
                                               bool recursive,
                                               QObject *parent)
    : QueueTask(1,
                parent),
      m_fis(std::vector<FileInfoBD*>()),
      m_order(order),
      m_recursive(recursive)
{
    m_fis.push_back(fiBD);
    connectSignals();
}

QueueTasks::TaskFolderSorter::~TaskFolderSorter()
{
}

void QueueTasks::TaskFolderSorter::run()
{
    if(m_recursive)
        emit sortFolders(m_order);
    else
        emit sortFolders_rec(m_order);
}
void QueueTasks::TaskFolderSorter::connectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        connect(this, &TaskFolderSorter::sortFolders,            m_fiBD, &FileInfoBD::sortFolder);
        connect(this, &TaskFolderSorter::sortFolders_rec,        m_fiBD, &FileInfoBD::sortFolder_rec);
        connect(this, &TaskFolderSorter::cancelSGNL,             m_fiBD, &FileInfoBD::cancelSorting);

        connect(m_fiBD, &FileInfoBD::sortingFinished,   this, &TaskFolderSorter::onFinished);
    }
}

void QueueTasks::TaskFolderSorter::disconnectSignals()
{
    for(auto* m_fiBD: m_fis)
    {
        disconnect(this, &TaskFolderSorter::sortFolders,            m_fiBD, &FileInfoBD::sortFolder);
        disconnect(this, &TaskFolderSorter::sortFolders_rec,        m_fiBD, &FileInfoBD::sortFolder_rec);
        disconnect(this, &TaskFolderSorter::cancelSGNL,             m_fiBD, &FileInfoBD::cancelElapsing);

        disconnect(m_fiBD, &FileInfoBD::sortingFinished,   this, &TaskFolderSorter::onFinished);
    }
}

//------------------------------------------------------------------------------------


QueueTasks::TaskFolderCloser::TaskFolderCloser(std::vector<FileInfoBD *> &fis,
                                               QObject *parent)
    : QueueTask(static_cast<int>(fis.size()),
                parent),
      m_fis(fis)
{
    connectSignals();
}

QueueTasks::TaskFolderCloser::TaskFolderCloser(FileInfoBD *fiBD,
                                               QObject *parent)
    : QueueTask(1,
                parent),
      m_fis(std::vector<FileInfoBD*>())
{
    m_fis.push_back(fiBD);
    connectSignals();
}

QueueTasks::TaskFolderCloser::~TaskFolderCloser()
{
}

void QueueTasks::TaskFolderCloser::run()
{
    emit closeFolders(true);
}


void QueueTasks::TaskFolderCloser::connectSignals()
{
    for(auto* fi: m_fis)
    {
        connect(this, &TaskFolderCloser::closeFolders, fi, &FileInfoBD::close);
//        connect(this, &TaskFolderCloser::cancelClosing, fi , &FileInfoBD::cancelClosing); // macht schlicht keinen sinn...

        connect(fi, &FileInfoBD::closingFinished, this, &TaskFolderCloser::onFinished);
    }
}

void QueueTasks::TaskFolderCloser::disconnectSignals()
{
    for(auto* fi: m_fis)
    {
        disconnect(this, &TaskFolderCloser::closeFolders, fi, &FileInfoBD::close);
//        connect(this, &TaskFolderCloser::cancelSGNL, fi , &FileInfoBD::cancelClosing); // macht schlicht keinen sinn...

        disconnect(fi, &FileInfoBD::closingFinished, this, &TaskFolderCloser::onFinished);
    }
}
