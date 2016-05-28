#-------------------------------------------------
#
# Project created by QtCreator 2016-05-28T19:04:35
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLightAndChill
TEMPLATE = app


SOURCES += main.cpp\
        lightandchill.cpp

HEADERS  += lightandchill.h

FORMS    += lightandchill.ui

win32: LIBS += -L$$PWD/dep/bass/ -lbass

INCLUDEPATH += $$PWD/dep/bass
DEPENDPATH += $$PWD/dep/bass

win32: LIBS += -L$$PWD/dep/basswasapi/ -lbasswasapi

INCLUDEPATH += $$PWD/dep/basswasapi
DEPENDPATH += $$PWD/dep/basswasapi
