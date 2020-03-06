#-------------------------------------------------
#
# Project created by QtCreator 2017-06-08T10:45:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BioApplication07
TEMPLATE = app


SOURCES += \
    creature.cpp \
    main.cpp \
    mainwindow.cpp \
    open-simplex-noise.cpp \
    simulationarea.cpp \
    tga.cpp \
    astar.cpp \
    priotilequeue.cpp \
    datamanager.cpp \
    ki.cpp

HEADERS  += \
    creature.hpp \
    mainwindow.hpp \
    myexceptions.hpp \
    open-simplex-noise.hpp \
    simulationarea.hpp \
    tga.hpp \
    creatureinstanz.hpp \
    astar.hpp \
    priotilequeue.hpp \
    utils.hpp \
    datamanager.hpp \
    ki.hpp

FORMS    += \
    mainwindow.ui
