QT       -= gui

CONFIG(debug, debug|release) : TARGET = MCSPlugind
CONFIG(release, debug|release) : TARGET = MCSPlugin

TEMPLATE = lib

CONFIG += c++11

DEFINES += MCS_LIBRARY

SOURCES += \
    MCSPluginContext.cpp \
    MCSPluginFramework.cpp

HEADERS += \
        mcs_global.h \
    MCSPluginActivator.h \
    MCSPluginContext.h \
    MCSPluginFramework.h

target.path = $$PWD/MCSPlugin/lib
headers.files = $$HEADERS
headers.path = $$PWD/MCSPlugin/include
other.files = $$PWD/MCSPlugin.pri create_plugin.py
other.path = $$PWD/MCSPlugin
INSTALLS += target headers other
