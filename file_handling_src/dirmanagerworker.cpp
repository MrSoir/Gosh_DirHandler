#include "dirmanagerworker.h"

DirManagerWorker::DirManagerWorker(QObject *parent)
    : QObject(parent),
      m_cancelled(false)
{
    // wird in start connected, und zwar nach dem thread:
//    connect(this, &DirManagerWorker::finished, this, &DirManagerWorker::deleteLater);
}

DirManagerWorker::~DirManagerWorker()
{
    qDebug() << "~DirManagerWorker";
}

void DirManagerWorker::cancel()
{
    m_cancelled = true;
    emit cancelled();
}

void DirManagerWorker::start()
{
    QThread* thread = new QThread();
    this->moveToThread(thread);

    connect(this, &DirManagerWorker::finished, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &DirManagerWorker::finished, this, &DirManagerWorker::deleteLater);

    thread->start();

    emit runTask();
}
