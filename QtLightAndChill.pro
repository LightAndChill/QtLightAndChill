#-------------------------------------------------
#
# Project created by QtCreator 2016-05-28T19:04:35
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtLightAndChill
TEMPLATE = app

SOURCES += main.cpp\
    http-parser/http_parser.c \
    qhttpserver/qhttpconnection.cpp \
    qhttpserver/qhttprequest.cpp \
    qhttpserver/qhttpresponse.cpp \
    qhttpserver/qhttpserver.cpp \
    lightandchill.cpp

HEADERS  += http-parser/http_parser.h \
    qhttpserver/qhttpconnection.h \
    qhttpserver/qhttprequest.h \
    qhttpserver/qhttpresponse.h \
    qhttpserver/qhttpserver.h \
    qhttpserver/qhttpserverapi.h \
    qhttpserver/qhttpserverfwd.h \
    lightandchill.h \

FORMS    += lightandchill.ui

# BASS

win32: LIBS += -L$$PWD/dep/bass/ -lbass

INCLUDEPATH += $$PWD/dep/bass
DEPENDPATH += $$PWD/dep/bass

# BASSWASAPI

win32: LIBS += -L$$PWD/dep/basswasapi/ -lbasswasapi

INCLUDEPATH += $$PWD/dep/basswasapi
DEPENDPATH += $$PWD/dep/basswasapi
