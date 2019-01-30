#ifndef PROCESS_H
#define PROCESS_H

#include <QProcess>
#include <QDebug>
#include <QObject>

// Process selbst implementiert: da im Constructor der QProcess sein signal finished(...) selbst direkt mit deletelater verbindet! -> somit entsteht nie ein haengeder pointer!

class Process: public QProcess
{
public:
    explicit Process();
    ~Process();
};
#endif // PROCESS_H
