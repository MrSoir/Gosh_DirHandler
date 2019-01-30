#include "queuetask.h"


QueueTask::QueueTask(int jobCounter,
                     QObject *parent)
    : QObject(parent),
      m_jobCounter(jobCounter),
      m_cancelled(false)
{
}

QueueTask::QueueTask(const QueueTask &task)
    : QObject(nullptr),
      m_jobCounter(task.m_jobCounter),
      m_cancelled(task.m_cancelled)
{
}

QueueTask::~QueueTask()
{
    this->thread()->quit();
}

bool QueueTask::executableInParallel() const
{
    return false;
}

bool QueueTask::cancelled() const
{
    return m_cancelled;
}

void QueueTask::pause()
{
}

void QueueTask::continue_()
{
}

void QueueTask::cancel()
{
    m_cancelled = true;
    emit cancelSGNL();
}

void QueueTask::onFinished()
{
    if(--m_jobCounter <= 0)
    {
        disconnectSignals();
        emit finished();
    }
}

void QueueTask::createThread()
{
    QThread* thread = new QThread();
    this->moveToThread(thread);

    connectThreadSignals(thread); // connecThreadSignals wird zum Teil von sub-klassen ueberschrieben, da dort nicht sofort die run-function beim start des threads loslaufen soll sondern erst spaeter -> z.B. beim TextDialog soll die run-function erst dann loslaufen, wenn der user den text ausgewaehlt hat und ok klickt!

    thread->start();
}

void QueueTask::connectSignals()
{
}

void QueueTask::disconnectSignals()
{
}

void QueueTask::connectThreadSignals(QThread* thread)
{
    connect(thread, &QThread::started, this, &QueueTask::run, Qt::QueuedConnection);
    connect(this, &QueueTask::finished, thread, &QThread::quit, Qt::QueuedConnection);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
}


