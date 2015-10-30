#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T10:36:29
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../bial/bial.pri)

TARGET = OpenGLView
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stlviewer.cpp

HEADERS  += mainwindow.h \
    stlviewer.h \
    glassert.h

FORMS    += mainwindow.ui

LIBS += -lGL -lGLU -lglut

CONFIG += c++11
