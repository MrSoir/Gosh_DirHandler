#ifndef DIRMANAGERQUEUE_H
#define DIRMANAGERQUEUE_H

#include <QObject>
#include <QDebug>

#include <vector>
#include <string>

#include "dirmanagerworker.h"

class DirManagerQueue : public QObject
{
    Q_OBJECT
public:
    explicit DirManagerQueue(QObject *parent = nullptr);
    virtual ~DirManagerQueue() override;

signals:
    void killWorkers();

    void revalidateDirStructure();
    void noMoreWorkersRunning();
public slots:
    void addWorker(DirManagerWorker* worker);
    void workerFinished(bool revalidateDirStructureAfterWorkerHasFinished);

    void dirStructureRevalidated();

    void close();
private:
    void startWorkersInQueue();

    void launchWorker(DirManagerWorker* worker);

    void cleanUp();

    int m_workersRunning;
    std::vector<DirManagerWorker*> m_workersToRun;

    bool m_closed;
};

#endif // DIRMANAGERQUEUE_H
