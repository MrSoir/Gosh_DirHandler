#include "process.h"

Process::Process()
    : QProcess()
{
    // dafuer sorgen, dass der process nach dem finished deleted wird:
    connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &QProcess::deleteLater);
}

Process::~Process()
{
    qDebug() << "~Process";
}
