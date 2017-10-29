TEMPLATE = app
CONFIG += console c++11 c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/cdataset.cpp \
    src/cevaluator.cpp \
    src/cmodelfactory.cpp \
    src/cbaseclassifier.cpp \
    src/cnaivebayesclassifier.cpp \
    src/crule.cpp \
    src/cilarulesextractor.cpp

HEADERS += \
    src/cdataset.h \
    src/cevaluator.h \
    src/cmodelfactory.h \
    src/cbaseclassifier.h \
    src/cnaivebayesclassifier.h \
    src/crule.h \
    src/cilarulesextractor.h

DEFINES += FILE_OUTPUT
#DEFINES += DEBUG_EVALUATOR
