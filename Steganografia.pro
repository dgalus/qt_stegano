#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T19:09:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Steganografia
TEMPLATE = app


SOURCES += main.cpp\
    Stegano.cpp \
    GraphWindow.cpp \
    SelectModeWindow.cpp \
    EncodeWindow.cpp \
    DecodeWindow.cpp \
    ViewImageWindow.cpp

HEADERS  += \
    TextConstants.h \
    Stegano.h \
    Enums.h \
    GraphWindow.h \
    SelectModeWindow.h \
    EncodeWindow.h \
    DecodeWindow.h \
    ViewImageWindow.h

FORMS    += \
    SelectModeWindow.ui \
    GraphWindow.ui \
    EncodeWindow.ui \
    DecodeWindow.ui \
    ViewImageWindow.ui
