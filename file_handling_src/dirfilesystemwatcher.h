#ifndef DIRFILESYSTEMWATCHER_H
#define DIRFILESYSTEMWATCHER_H

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include <string>
#include <vector>

class DirFileSystemWatcher : public QObject
{
    Q_OBJECT
public:
    explicit DirFileSystemWatcher(QObject *parent = nullptr);

signals:
    void dirChanged(std::string dir);
public slots:
    void addDir(const std::string& dir_path);
    void addDirs(const std::vector<std::string> &dir_paths);

    void clearDirs();
private:
    void connectSignals();

    void dirChanged_hlpr(const QString& q_dir);

    QFileSystemWatcher* m_watcher;
    std::vector<std::string> m_dirs_watched;
};

#endif // DIRFILESYSTEMWATCHER_H
