INCLUDEPATH += $$PWD/include

CONFIG(debug, debug|release) : LIBS += -L$$PWD/lib -lMCSPlugind
CONFIG(release, debug|release) : LIBS += -L$$PWD/lib -lMCSPlugin
