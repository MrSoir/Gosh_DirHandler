#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QObject>
#include <QString>
#include <QFileSystemWatcher>
#include <string>

class FileWatcher : public QObject
{
    Q_OBJECT
public:
    explicit FileWatcher(QObject *parent = nullptr);
    ~FileWatcher();

signals:
    void directoryChanged(std::string path);
public slots:
    void directoryChanged_slot(const QString& str);

    void addPath(std::string path);
private:
    QFileSystemWatcher* m_watcher;
};

#endif // FILEWATCHER_H
