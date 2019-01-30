#ifndef FILEQUEUE_H
#define FILEQUEUE_H

#include <QObject>
#include <queue>
#include <functional>

#include "queuetask.h"

class FileQueue : public QObject
{
    Q_OBJECT
public:
    explicit FileQueue();
    ~FileQueue();
signals:
    void paused();
    void continued();
    void cancelled();

    // teilt dem FileManager mit, in die warteschleife zu gehen und diese wieder zu beenden:
    void blockUntilTaskHasFinish();
    void blockingTaskHasFinished();
public slots:
    void addTask(QueueTask* task);

    void pause();
    void continue_();
    void cancel();

    void taskFinished();
private:
    void processTasks();

    void connectTask(QueueTask* task);
    void disconnectTask(QueueTask* task);

    std::queue<QueueTask*> m_tasks;

    bool m_taskIsRunning;
};

#endif // FILEQUEUE_H
