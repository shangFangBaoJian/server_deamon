QT += core  websockets network
QT -= gui

CONFIG += c++11

TARGET = datafirewall_refect
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    business.cpp \
    datahandler.cpp \
    hmhttp.cpp \
    hmlog.cpp \
    hmutils.cpp \
    server.cpp \
    tcpserver.cpp

HEADERS += \
    business.h \
    datahandler.h \
    hmhttp.h \
    hmlog.h \
    hmutils.h \
    server.h \
    tcpserver.h
