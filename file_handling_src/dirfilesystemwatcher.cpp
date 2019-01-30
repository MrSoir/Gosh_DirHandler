#include "dirfilesystemwatcher.h"

DirFileSystemWatcher::DirFileSystemWatcher(QObject* parent)
    : QObject(parent),
      m_watcher(new QFileSystemWatcher()),
      m_dirs_watched(std::vector<std::string>())
{
    connectSignals();
}

void DirFileSystemWatcher::addDir(const std::string& dir_path)
{
    m_watcher->addPath(QString::fromStdString(dir_path));
    m_dirs_watched.push_back(dir_path);
}

void DirFileSystemWatcher::addDirs(const std::vector<std::string>& dir_paths)
{
    for(const auto& dir_path: dir_paths)
    {
        m_watcher->addPath(QString::fromStdString(dir_path));
        m_dirs_watched.push_back(dir_path);
    }
}

void DirFileSystemWatcher::clearDirs()
{
    m_watcher->removePaths(m_watcher->directories());
    m_dirs_watched.clear();
}

void DirFileSystemWatcher::connectSignals()
{
    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this, &DirFileSystemWatcher::dirChanged_hlpr);
}

void DirFileSystemWatcher::dirChanged_hlpr(const QString& q_dir)
{
    emit dirChanged(q_dir.toStdString());
}
