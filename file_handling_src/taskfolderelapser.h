#ifndef TASKFOLDERELAPSER_H
#define TASKFOLDERELAPSER_H

#include <QObject>
#include "filequeue.h"
#include "fileinfobd.h"

//class TaskFolderElapser : public QueueTask
//{
//    Q_OBJECT
//public:
//    explicit TaskFolderElapser(FileInfoBD* fiBD,
//                               bool collapse = false,
//                               bool recursively = false,
//                               QObject *parent = nullptr);
//signals:
//    void finished(QueueTask*);

//    void elapseFiBD();
//    void elapseFiBD_rec();
//    void collapseFiBD();
//    void collapseFiBD_rec();
//    void cancelElapsing();
//    void cancelCollapsing();
//public slots:
//    void run();
//    void pause(); // pause nicht implementiert == tut einfach nichts
//    void continue_(); // continue nicht implementiert, da pause auch nicht implementiert (tut einfach nichts)
//    void cancel();

//    void elapsingFinished();
//    void collapsingFinished();
//public:
//    bool executableInParallel() const;
//private:
//    void connectFiBD();
//    void disconnectFiBD();

//    FileInfoBD* m_fiBD;
//    bool m_collapse;
//    bool m_recursively;
//};

#endif // TASKFOLDERELAPSER_H
