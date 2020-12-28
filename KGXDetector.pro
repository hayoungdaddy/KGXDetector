#-------------------------------------------------
#
# Project created by QtCreator 2016-11-11T14:17:06
#
#-------------------------------------------------

QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KGXDetector
TEMPLATE = app

LIBS    += -lzmq

SOURCES += main.cpp\
        mainwindow.cpp \
    config.cpp \
    qcustomplot.cpp \
    rvcoeffzmq.cpp \
    rvpickzmq.cpp \
    viewpickreport.cpp \
    eventlist.cpp \
    briefingboard.cpp \
    mysnapshot.cpp \
    showallpicks.cpp \
    mbgraph.cpp \
    calllocation.cpp \
    locgraph.cpp \
    calllocationnogui.cpp

HEADERS  += mainwindow.h \
    config.h \
    common.h \
    qcustomplot.h \
    rvcoeffzmq.h \
    rvpickzmq.h \
    viewpickreport.h \
    eventlist.h \
    briefingboard.h \
    mysnapshot.h \
    showallpicks.h \
    mbgraph.h \
    calllocation.h \
    locgraph.h \
    calllocationnogui.h

FORMS    += mainwindow.ui \
    config.ui \
    viewpickreport.ui \
    eventlist.ui \
    briefingboard.ui \
    mysnapshot.ui \
    showallpicks.ui \
    mbgraph.ui \
    locgraph.ui

LIBS += -L$$PWD/../util/qlib2/ -lqlib2-64
INCLUDEPATH += $$PWD/../util/qlib2
DEPENDPATH += $$PWD/../util/qlib2

LIBS += -L$$PWD/../../../../opt/IDCSW/lib/ -lmagnitude -lgeog -lgresp -lgobject -lstring -linterp -lFT -lmath -lgerror -lsyslog
INCLUDEPATH += $$PWD/../../../../opt/IDCSW/include
DEPENDPATH += $$PWD/../../../../opt/IDCSW/include
INCLUDEPATH += $$PWD/../../../../opt/IDCSW/include/ibase
DEPENDPATH += $$PWD/../../../../opt/IDCSW/include/ibase

