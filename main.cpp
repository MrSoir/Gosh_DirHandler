#include "mainwindow.h"
#include <QApplication>

#include <QString>

#include <vector>
#include <string>

#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void handler(int sig)
{
  void *array[10];
  int size;

  // get void*'s for all entries on the stack
  size = backtrace(array, 10);

  // print out all the frames to stderr
  fprintf(stderr, "Error: signal %d:\n", sig);
  backtrace_symbols_fd(array, size, STDERR_FILENO);
  exit(0);
}

Q_DECLARE_METATYPE(QString);
Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(std::vector<std::string>);
Q_DECLARE_METATYPE(std::vector<QString>);
Q_DECLARE_METATYPE(FileInfoBD);
Q_DECLARE_METATYPE(std::vector<FileInfoBD*>);
Q_DECLARE_METATYPE(FiBDDeletor*);
Q_DECLARE_METATYPE(std::vector<FiBDDeletor*>);
Q_DECLARE_METATYPE(DeepSearchResult);
Q_DECLARE_METATYPE(std::vector<DeepSearchResult>);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    signal(SIGSEGV, handler);

    qRegisterMetaType<QString>();
    qRegisterMetaType<std::string>();
    qRegisterMetaType<std::vector<std::string>>();
    qRegisterMetaType<std::vector<QString>>();
    qRegisterMetaType<FileInfoBD>();
    qRegisterMetaType<std::vector<FileInfoBD*>>();
    qRegisterMetaType<FiBDDeletor*>();
    qRegisterMetaType<std::vector<FiBDDeletor*>>();
    qRegisterMetaType<DeepSearchResult>();
    qRegisterMetaType<std::vector<DeepSearchResult>>();

    MainWindow w;
    w.show();

    return a.exec();
}
