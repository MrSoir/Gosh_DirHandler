#include "filewatcher.h"

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent),
      m_watcher(new QFileSystemWatcher(this))
{
    connect(m_watcher, &QFileSystemWatcher::directoryChanged,
            this,      &FileWatcher::directoryChanged_slot);
}

FileWatcher::~FileWatcher()
{
    m_watcher->removePaths(m_watcher->directories());
    m_watcher->removePaths(m_watcher->files());
    delete m_watcher;
}

void FileWatcher::directoryChanged_slot(const QString& str)
{
    emit directoryChanged(str.toStdString());
}

void FileWatcher::addPath(std::string path)
{
    m_watcher->addPath(QString::fromStdString(path));
}
