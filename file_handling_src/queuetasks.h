#ifndef QUEUETASKS_H
#define QUEUETASKS_H

#include <QObject>
#include <string>
#include <vector>

#include "filequeue.h"
#include "queuetask.h"
#include "fileinfobd.h"
#include "orderby.h"

namespace QueueTasks
{
    class FileInfoBDRevalidator: public QueueTask
    {
        Q_OBJECT
    public:
        FileInfoBDRevalidator(FileInfoBD* fibd,
                              QObject* parent = nullptr);
        FileInfoBDRevalidator(std::vector<FileInfoBD*>& fis,
                              QObject* parent = nullptr);
        FileInfoBDRevalidator(const FileInfoBDRevalidator& cpy);
        ~FileInfoBDRevalidator() override;
    signals:
        void revalidateFiBDs();
    public slots:
        void run() override;
    private:
        void connectSignals() override;
        void disconnectSignals() override;

        std::vector<FileInfoBD*> m_fis;
    };

//-------------------------------------------------------------------------------------------------

    class TaskFolderElapser: public QueueTask
    {
        Q_OBJECT
    public:
        explicit TaskFolderElapser(std::vector<FileInfoBD*>& fis,
                                   bool collapse = false,
                                   bool recursively = false,
                                   QObject *parent = nullptr);
        explicit TaskFolderElapser(std::unordered_set<FileInfoBD*>& fis,
                                   bool collapse = false,
                                   bool recursively = false,
                                   QObject *parent = nullptr);
        explicit TaskFolderElapser(FileInfoBD* fiBD,
                                   bool collapse = false,
                                   bool recursively = false,
                                   QObject *parent = nullptr);
        virtual ~TaskFolderElapser() override;

    signals:
        void elapseFiBD();
        void elapseFiBD_rec();
        void collapseFiBD();
        void collapseFiBD_rec();
    public slots:
        void run() override;
    public:
    private:
        void connectSignals() override;
        void disconnectSignals() override;

        std::vector<FileInfoBD*> m_fis;
        bool m_collapse;
        bool m_recursively;

    };

//-------------------------------------------------------------------------------------------------

    class TaskShowHiddenFiles : public QueueTask
    {
        Q_OBJECT
    public:
        explicit TaskShowHiddenFiles(std::vector<FileInfoBD*>& fiBD,
                                     bool hide = false,
                                     bool recursive = false,
                                     QObject *parent = nullptr);
        explicit TaskShowHiddenFiles(FileInfoBD* fiBD,
                                     bool hide = false,
                                     bool recursive = false,
                                     QObject *parent = nullptr);
        virtual ~TaskShowHiddenFiles() override;
    signals:
        void hideHiddenFilesFiBD();
        void hideHiddenFilesFiBD_rec();
        void showHiddenFilesFiBD();
        void showHiddenFilesFiBD_rec();
    public slots:
        void run() override;
    public:
    private:
        void connectSignals() override;
        void disconnectSignals() override;

        std::vector<FileInfoBD*> m_fis;
        bool m_hide;
        bool m_recursive;
    };

    class TaskFolderSorter : public QueueTask
    {
        Q_OBJECT
    public:
        explicit TaskFolderSorter(std::vector<FileInfoBD*>& fis,
                                  Order order,
                                  bool recursive,
                                  QObject* parent = nullptr);
        explicit TaskFolderSorter(FileInfoBD* fiBD,
                                  Order order,
                                  bool recursive,
                                  QObject* parent = nullptr);
        virtual ~TaskFolderSorter() override;
    signals:
        void sortFolders(Order);
        void sortFolders_rec(Order);
    public slots:
        void run() override;
    public:
    private:
        void connectSignals() override;
        void disconnectSignals() override;

        std::vector<FileInfoBD*> m_fis;
        Order m_order;
        bool m_recursive;
    };

    class TaskFolderCloser : public QueueTask
    {
        Q_OBJECT
    public:
        explicit TaskFolderCloser(std::vector<FileInfoBD*>& fis,
                                  QObject* parent = nullptr);
        explicit TaskFolderCloser(FileInfoBD* fiBD,
                                  QObject* parent = nullptr);
        virtual ~TaskFolderCloser() override;
    signals:
        void closeFolders(bool removeFromParent);
    public slots:
        void run() override;
    public:
    private:
        void connectSignals() override;
        void disconnectSignals() override;

        std::vector<FileInfoBD*> m_fis;
    };
}



#endif // QUEUETASKS_H
