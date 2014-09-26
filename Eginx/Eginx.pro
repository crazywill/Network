TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    Eginx.cpp \
    Implementation/Master.cpp \
    Implementation/Worker.cpp \
    Implementation/Configreader.cpp \
    Implementation/Egx_ProcMutex.cpp

HEADERS += \
    Eginx.h \
    Utility/ScopedPtr.h \
    Utility/PackDef.h \
    Interface/IWorker.h \
    Interface/IProcessConnection.h \
    Interface/IComponent.h \
    Interface/IExecutor.h \
    Implementation/Master.h \
    Implementation/Worker.h \
    Interface/IConfigReader.h \
    Implementation/Configreader.h \
    Utility/PubDefine.h \
    Utility/Log.h \
    Interface/Egx_IProcMutex.h \
    Implementation/Egx_ProcMutex.h

LIBS += -lrt
