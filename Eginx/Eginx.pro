TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    Eginx.cpp

HEADERS += \
    Eginx.h \
    Utility/ScopedPtr.h \
    Utility/PackDef.h \
    Interface/IWorker.h \
    Interface/IProcessConnection.h

