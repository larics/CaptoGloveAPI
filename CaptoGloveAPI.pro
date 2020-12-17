QT += core bluetooth
QT -= gui

CONFIG += c+11
CONFIG += qt

# For final version without debug
# CONFIG += CAPTOGLOVEAPI_LIBRARY
# DEFINES += QT_NO_DEBUG_OUTPUT


TARGET = CaptoGloveAPI

CAPTOGLOVEAPI_LIBRARY {
    TEMPLATE=lib
    CONFIG += static
    DEFINES += CAPTOGLOVEAPI_LIBRARY
}else{
    CONFIG += console
    SOURCES += main.cpp
}

CONFIG += console

DEFINES += LINUX

DEFINES += PROJECT_PATH=\"\\\"$${_PRO_FILE_PWD_}/\\\"\"



SOURCES = captogloveapi.cpp\
          deviceinfo.cpp \
          serviceinfo.cpp \
          characteristicinfo.cpp \
          main.cpp

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



 
