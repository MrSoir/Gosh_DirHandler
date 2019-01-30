#ifndef QUEUETASK_H
#define QUEUETASK_H


#include <QObject>
#include <QString>
#include <QThread>

#include <queue>
#include <functional>

// GAAANZ WICHTIGER HINWEIS:
// FileQueue geht folgendermassen vor:
//      wenn es einen QueueTask cancelled, dann wird erwartet, dass der QueueTask
//      dennoch finished, d.h. das signal finished(QueueTask*) sendet.
//      -> nur eben etwas frueher, da durch das cancellen, die routine QueueTask::run()
//      frueher zum abschluss kommen sollte!
// das macht das ganze etwas einfacher zu programmieren. somit muss ausschliesslich
// das signal QueueTask::finished(QueueTask*) => FileQueue::taskFinished(QueueTask*) abgefangen werden und nicht auch noch
// das signal QueueTask::cancelled(QueueTask*) => FileQueue::taskCancelled(QueueTask*)
// semantisch bleibt es aber natuerlich fragwuerdig, nach einem canceln noch zu finishen...

class QueueTask : public QObject
{
    Q_OBJECT
public:
    QueueTask(int jobCounter = 0,
              QObject* parent = nullptr);
    QueueTask(const QueueTask& task);
    virtual ~QueueTask();

    virtual bool executableInParallel() const; // FileQueue muss QueueTask nicht auf Queue-Stack legen sondern kann ihn direkt starten, selbst wenn ein anderer Task laeuft. Beispiel: kopieren von dateien kann parallel ablaufen, da die aufgabe unabhaengig vom FileExplorer ist. Hingegen muss FileInfoBD.elapseAll() zwingend auf den Queue gepackt werden.

    bool cancelled() const;

    // default constructor & copy constructor & destructor sind gefordert, wenn QueueTask zwecks signal-slot angemeldet/registriert werden muss:
protected:
    int m_jobCounter; // protected, damit sub-classes direkten zugriff drauf haben

signals:
    void finished();
    void cancelSGNL();
public slots:
    virtual void run() = 0;
    virtual void pause();
    virtual void continue_();
    virtual void cancel();

    virtual void onFinished();

    virtual void createThread(); // Worker soll sich selbst in den QThread legen und diesen startetn bequem per aufruf createThread -> damit muss die Queue nur noch QueueTask::createThread() aufrufen und die Sache ist erledigt!
private:
    virtual void connectSignals();
    virtual void disconnectSignals();

protected:
    virtual void connectThreadSignals(QThread* thread);

    bool m_cancelled;
};


#endif // QUEUETASK_H
