#include "taskshowhiddenfiles.h"

//TaskShowHiddenFiles::TaskShowHiddenFiles(FileInfoBD *fiBD,
//                                         bool hide,
//                                         bool recursive,
//                                         QObject *parent)
//    : QueueTask(parent),
//      m_fiBD(fiBD),
//      m_hide(hide),
//      m_recursive(recursive)
//{
//    connectFiBD();
//}

//void TaskShowHiddenFiles::run()
//{
//    if(m_hide)
//    {
//        if(m_recursive)
//            emit hideHiddenFilesFiBD();
//        else
//            emit hideHiddenFilesFiBD_rec();
//    }else{
//        if(m_recursive)
//            emit showHiddenFilesFiBD();
//        else
//            emit showHiddenFilesFiBD_rec();
//    }
//}

//void TaskShowHiddenFiles::pause()
//{
//}

//void TaskShowHiddenFiles::continue_()
//{
//}

//void TaskShowHiddenFiles::cancel()
//{
//    emit cancelHidingFiles();
//}

//void TaskShowHiddenFiles::hidingFinished()
//{
//    disconnectFiBD();
//    emit finished(static_cast<QueueTask*>(this));
//}

//bool TaskShowHiddenFiles::executableInParallel() const
//{
//    return false;
//}

//void TaskShowHiddenFiles::connectFiBD()
//{
//    connect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD,       m_fiBD, &FileInfoBD::hideHiddenFiles);
//    connect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD_rec,   m_fiBD, &FileInfoBD::hideHiddenFiles_rec);
//    connect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD,     m_fiBD, &FileInfoBD::showHiddenFiles);
//    connect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD_rec, m_fiBD, &FileInfoBD::showHiddenFiles_rec);
//    connect(this, &TaskShowHiddenFiles::cancelHidingFiles,   m_fiBD, &FileInfoBD::cancelHidingHiddenFiles);

//    connect(m_fiBD, &FileInfoBD::hidingHiddenFilesFinished,   this, &TaskShowHiddenFiles::hidingFinished);
//}

//void TaskShowHiddenFiles::disconnectFiBD()
//{
//    disconnect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD,       m_fiBD, &FileInfoBD::hideHiddenFiles);
//    disconnect(this, &TaskShowHiddenFiles::hideHiddenFilesFiBD_rec,   m_fiBD, &FileInfoBD::hideHiddenFiles_rec);
//    disconnect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD,     m_fiBD, &FileInfoBD::showHiddenFiles);
//    disconnect(this, &TaskShowHiddenFiles::showHiddenFilesFiBD_rec, m_fiBD, &FileInfoBD::showHiddenFiles_rec);
//    disconnect(this, &TaskShowHiddenFiles::cancelHidingFiles,   m_fiBD, &FileInfoBD::cancelHidingHiddenFiles);

//    disconnect(m_fiBD, &FileInfoBD::hidingHiddenFilesFinished,   this, &TaskShowHiddenFiles::hidingFinished);
//}
