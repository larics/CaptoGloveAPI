QT += core bluetooth
QT -= gui

CONFIG += c+11
CONFIG += qt

TARGET = CaptoGloveAPI

CONFIG += console

DEFINES += LINUX

DEFINES += PROJECT_PATH=\"\\\"$${_PRO_FILE_PWD_}/\\\"\"

DEFINES += QT_NO_DEBUG_OUTPUT


SOURCES = main.cpp \
          captogloveapi.cpp \
          deviceinfo.cpp \
          serviceinfo.cpp \
          characteristicinfo.cpp

HEADERS = captogloveapi.h \
          deviceinfo.h \
          serviceinfo.h \
          characteristicinfo.h

# Linking
unix{

    LIBS += -pthread

    #INCLUDEPATH += $$PWD/../CaptoGloveLib/
    #LIBS += -L$$PWD/../CaptoGloveLib -lGSdkCoreStatic

}

QMAKE_CXXFLAGS += -std=gnu++0x -pthread
QMAKE_CFLAGS += -std=gnu++0x -pthread



 
