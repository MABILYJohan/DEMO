#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tp02
TEMPLATE = app


SOURCES += main.cpp\
        princ.cpp \
        glarea.cpp \
    convertvbo.cpp \
    utils.cpp \
    poisson.cpp \
    banc.cpp \
    obstacle.cpp \
    predateur.cpp

HEADERS  += princ.h \
        glarea.h \
    convertvbo.h \
    utils.h \
    poisson.h \
    banc.h \
    obstacle.h \
    predateur.h

FORMS    += princ.ui

RESOURCES += \
    tp02.qrc

DISTFILES +=
