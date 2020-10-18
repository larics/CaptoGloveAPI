QT += core
QT -= gui

# Add flags for pthread support
QMAKE_CXXFLAGS += -std=c++0x -pthread

CONFIG += c+11
CONFIG += qt debug

TARGET = CaptoGloveAPI

CONFIG += console

DEFINES += LINUX

SOURCES = main.cpp \
          BasicConnection.cpp \
          BasicExample.cpp \
          captogloveapi.cpp

HEADERS = BasicConnection.h \
          BasicExample.h \
          captogloveapi.h


message($$PWD/../CaptoGloveLib/include)
unix{

    LIBS += -pthread

    INCLUDEPATH += $$PWD/../CaptoGloveLib/
    LIBS += -L$$PWD/../CaptoGloveLib -lGSdkCoreStatic

}





 
