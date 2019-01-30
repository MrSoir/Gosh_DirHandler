#-------------------------------------------------
#
# Project created by QtCreator 2019-01-20T15:11:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileManager
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        orderby.cpp \
        pathmanipulation.cpp \
        process.cpp \
        staticfunctions.cpp \
        staticfunctions_files.cpp \
        stringops.cpp \
        file_handling_src/fileinfobd.cpp \
        file_handling_src/fileselector.cpp \
        file_handling_src/filesearcher.cpp \
        file_handling_src/fibdviewer.cpp \
        file_handling_src/filequeue.cpp \
        file_handling_src/filewatcher.cpp \
        file_handling_src/filemimetypehandler.cpp \
        file_handling_src/listfiles.cpp \
        file_handling_src/exec.cpp \
        file_handling_src/queuetasks.cpp \
        file_handling_src/appinfo.cpp \
        file_handling_src/searchable.cpp \
        file_handling_src/selectable.cpp \
        file_handling_src/queuetask.cpp \
        file_handling_src/openfiles.cpp \
    file_handling_src/dirmanager.cpp \
    file_handling_src/dirmanagerqueue.cpp \
    file_handling_src/dirmanagerworker.cpp \
    file_handling_src/dirdeleteworker.cpp \
    file_handling_src/dirrevalworker.cpp \
    file_handling_src/direlapseworker.cpp \
    file_handling_src/dirdeepsearchworker.cpp \
    file_handling_src/dirfilesystemwatcher.cpp


HEADERS += \
        mainwindow.h \
        orderby.h \
        orientation.h \
        pathmanipulation.h \
        process.h \
        staticfunctions.h \
        staticfunctions_files.h \
        stringops.h \
        file_handling_src/fileinfobd.h \
        file_handling_src/fileselector.h \
        file_handling_src/filesearcher.h \
        file_handling_src/fibdviewer.h \
        file_handling_src/filequeue.h \
        file_handling_src/filewatcher.h \
        file_handling_src/filemimetypehandler.h \
        file_handling_src/listfiles.h \
        file_handling_src/exec.h \
        file_handling_src/queuetasks.h \
        file_handling_src/appinfo.h \
        file_handling_src/searchable.h \
        file_handling_src/selectable.h \
        file_handling_src/queuetask.h \
        file_handling_src/openfiles.h \
    file_handling_src/dirmanager.h \
    file_handling_src/dirmanagerqueue.h \
    file_handling_src/dirmanagerworker.h \
    file_handling_src/dirdeleteworker.h \
    file_handling_src/dirrevalworker.h \
    file_handling_src/direlapseworker.h \
    file_handling_src/dirdeepsearchworker.h \
    file_handling_src/dirfilesystemwatcher.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
