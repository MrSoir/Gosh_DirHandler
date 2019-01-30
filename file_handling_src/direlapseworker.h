#ifndef ELAPSEWORKER_H
#define ELAPSEWORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include <vector>
#include <string>

#include "dirmanagerworker.h"
#include "fileinfobd.h"
#include "staticfunctions_files.h"

class DirElapseWorker : public DirManagerWorker
{
    Q_OBJECT
public:
    explicit DirElapseWorker(std::vector<FileInfoBD*> dirsToElapse,
                          bool recursive,
                          bool collapse);
    explicit DirElapseWorker(FileInfoBD* dirToElapse,
                          bool recursive,
                          bool collapse);
    virtual ~DirElapseWorker() override;

    virtual bool blockOtherThreads() const override;
    virtual bool revalidateDirStructureAfterWorkerHasFinished() const override;

signals:
    void killHelpers();
public slots:
    void run();
    void workerFinished(FileInfoBD* dir);
private:
    void connectSignals();

    void collapseHelper(FileInfoBD* dir);

    std::vector<FileInfoBD*> m_dirsToElapse;
    bool m_recursive;
    bool m_collapse;
    bool m_useSubThreads; // collapsing verwendet keine sub-threads, da hier nur die flags umgesetzt werden. beim elapsen hingegen sind ggfs. ordnerstrukturen neu zu laden, was zeitintensiv sein kann
    int m_runningThreads;
};


//----------------------------------------------------------------------------------------


class DirElapseWorkerHelper: public QObject
{
    Q_OBJECT
public:
    explicit DirElapseWorkerHelper(FileInfoBD* dir,
                       bool recursive);
    virtual ~DirElapseWorkerHelper() override;

signals:
    void finished(FileInfoBD* fi);
public slots:
    void run();
    void cancel();
private:
    void elapseHelper(FileInfoBD* dir);

    FileInfoBD* m_dir;
    bool m_recursive;
    bool m_cancelled;
};


#endif // ELAPSEWORKER_H
