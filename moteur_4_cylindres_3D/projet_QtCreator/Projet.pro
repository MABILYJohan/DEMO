#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

LIBS += -lGLU

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Projet
TEMPLATE = app


SOURCES += main.cpp\
        princ.cpp \
        glarea.cpp \
    utils.cpp

HEADERS  += princ.h \
        glarea.h \
    utils.h

FORMS    += princ.ui

RESOURCES += \
    Projet.qrc
