#ifndef TASKSHOWHIDDENFILES_H
#define TASKSHOWHIDDENFILES_H

#include <QObject>
#include "filequeue.h"
#include "fileinfobd.h"

//class TaskShowHiddenFiles : public QueueTask
//{
//    Q_OBJECT
//public:
//    explicit TaskShowHiddenFiles(FileInfoBD* fiBD,
//                                 bool hide = false,
//                                 bool recursive = false,
//                                 QObject *parent = nullptr);

//signals:
//    void finished(QueueTask*);

//    void hideHiddenFilesFiBD();
//    void hideHiddenFilesFiBD_rec();
//    void showHiddenFilesFiBD();
//    void showHiddenFilesFiBD_rec();
//    void cancelHidingFiles();
//public slots:
//    void run();
//    void pause(); // pause nicht implementiert == tut einfach nichts
//    void continue_(); // continue nicht implementiert, da pause auch nicht implementiert (tut einfach nichts)
//    void cancel();

//    void hidingFinished();
//public:
//    bool executableInParallel() const;
//private:
//    void connectFiBD();
//    void disconnectFiBD();

//    FileInfoBD* m_fiBD;
//    bool m_hide;
//    bool m_recursive;
//};

#endif // TASKSHOWHIDDENFILES_H
