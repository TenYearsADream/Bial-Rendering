QT += testlib
TARGET = Bial_Render_Test
CONFIG += console

include(../../bial/bial.pri)


SOURCES += \
    main.cpp \
    testgeometrics.cpp \
    testmarchingcubes.cpp \
    testdraw.cpp

HEADERS += \
    testgeometrics.h \
    testmarchingcubes.h \
    testdraw.h
