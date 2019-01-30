#ifndef DIRDELETEWORKER_H
#define DIRDELETEWORKER_H

#include <QObject>
#include <QDebug>
#include <QThread>

#include <vector>
#include <string>

#include "dirmanagerworker.h"
#include "fileinfobd.h"
#include "staticfunctions_files.h"

class DirDeleteWorker: public DirManagerWorker
{
    Q_OBJECT
public:
    explicit DirDeleteWorker(std::vector<FiBDDeletor*> dirsToElapse);
    explicit DirDeleteWorker(FiBDDeletor* dirToDelete);
    virtual ~DirDeleteWorker() override;

    virtual bool blockOtherThreads() const override;
    virtual bool revalidateDirStructureAfterWorkerHasFinished() const override;
signals:
public slots:
    void run();
private:
    void connectSignals();

    void deleteHelper(FiBDDeletor* dir);

    void evalDirsToDelete();

    std::vector<FiBDDeletor*> m_dirsToDelete;
};

#endif // DIRDELETEWORKER_H
