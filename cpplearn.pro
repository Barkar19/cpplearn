TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    cdataset.cpp \
    cevaluator.cpp \
    cmodelfactory.cpp \
    cbaseclassifier.cpp

HEADERS += \
    cdataset.h \
    cevaluator.h \
    cmodelfactory.h \
    cbaseclassifier.h
