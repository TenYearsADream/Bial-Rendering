QT += testlib
TARGET = Bial_Render
CONFIG += console

include(../bial/bial.pri)

INCLUDEPATH += inc


SOURCES += \
    main.cpp \
    tst/testgeometrics.cpp \
    tst/testmarchingcubes.cpp \
    tst/testdraw.cpp

HEADERS += \
    tst/testgeometrics.h \
    tst/testmarchingcubes.h \
    tst/testdraw.h
