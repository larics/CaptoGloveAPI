QT += core
QT -= gui

CONFIG += c+11
CONFIG += qt debug

TARGET = CaptoGloveAPI

CONFIG += console

DEFINES += LINUX

SOURCES = main.cpp \
          AdapterImpl_Linux.cpp \
          BasicConnection.cpp \
          BasicExample.cpp \
          captogloveapi.cpp

HEADERS = BasicConnection.h \
          BasicExample.h \
          captogloveapi.h

# Linking
unix{

    LIBS += -pthread

    INCLUDEPATH += $$PWD/../CaptoGloveLib/
    LIBS += -L$$PWD/../CaptoGloveLib -lGSdkCoreStatic

}

QMAKE_CXXFLAGS += -std=gnu++0x -pthread
QMAKE_CFLAGS += -std=gnu++0x -pthread



 
