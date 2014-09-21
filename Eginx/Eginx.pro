TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    Eginx.cpp \
    Implementation/Master.cpp \
    Implementation/Worker.cpp

HEADERS += \
    Eginx.h \
    Utility/ScopedPtr.h \
    Utility/PackDef.h \
    Interface/IWorker.h \
    Interface/IProcessConnection.h \
    Interface/IComponent.h \
    Interface/IExecutor.h \
    Implementation/Master.h \
    Implementation/Worker.h

