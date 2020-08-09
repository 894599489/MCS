#!/usr/bin/env python

import sys
import os


def createPro(pluginName):
    with open('{}.pro'.format(pluginName), 'w+') as fp:
        fp.write('TARGET = com.atomdata.mcs.{}\n'.format(pluginName.lower()))
        fp.write('TEMPLATE = lib\n')
        fp.write('\n')
        fp.write('CONFIG += plugin\n')
        fp.write('CONFIG += c++11\n')
        fp.write('\n')
        fp.write('RESOURCES += res.qrc\n')
        fp.write('\n')
        fp.write('include(MCSPlugin.pri)\n')
        fp.write('\n')
        fp.write('HEADERS += \\\n')
        fp.write('    {}Activator.h\n'.format(pluginName))
        fp.write('\n')
        fp.write('SOURCES += \\\n')
        fp.write('    {}Activator.cpp\n'.format(pluginName))
        fp.write('\n')


def createActivatorH(pluginName):
    with open('{}Activator.h'.format(pluginName), 'w+') as fp:
        fp.write('#ifndef {}ACTIVATOR_H\n'.format(pluginName.upper()))
        fp.write('#define {}ACTIVATOR_H\n'.format(pluginName.upper()))
        fp.write('\n')
        fp.write('#include "MCSPluginActivator.h"\n')
        fp.write('\n')
        fp.write('class {}Activator : public QObject, public MCSPluginActivator\n'.format(pluginName))
        fp.write('{\n')
        fp.write('    Q_OBJECT\n')
        fp.write('    Q_INTERFACES(MCSPluginActivator)\n')
        fp.write('    Q_PLUGIN_METADATA(IID "com.atomdata.mcs.{}")\n'.format(pluginName.lower()))
        fp.write('public:\n')
        fp.write('    virtual void start(MCSPluginContext *context) override;\n')
        fp.write('    virtual void stop(MCSPluginContext *context) override;\n')
        fp.write('};\n')
        fp.write('\n')
        fp.write('#endif // {}ACTIVATOR_H'.format(pluginName.upper()))


def createActivatorCpp(pluginName):
    with open('{}Activator.cpp'.format(pluginName), 'w+') as fp:
        fp.write('#include "{}Activator.h"\n'.format(pluginName))
        fp.write('\n')
        fp.write('void {}Activator::start(MCSPluginContext *context)\n'.format(pluginName))
        fp.write('{\n')
        fp.write('    \n')
        fp.write('}\n')
        fp.write('\n')
        fp.write('void {}Activator::stop(MCSPluginContext *context)\n'.format(pluginName))
        fp.write('{\n')
        fp.write('    \n')
        fp.write('}\n')
        fp.write('\n')


def createQrc(pluginName):
    with open('res.qrc', 'w+') as fp:
        fp.write('<RCC>\n')
        fp.write('    <qresource prefix="/com.atomdata.mcs.{}/META-INF">\n'.format(pluginName.lower()))
        fp.write('        <file>MANIFEST.MF</file>\n')
        fp.write('    </qresource>\n')
        fp.write('</RCC>\n')


def createManifest(pluginName):
    with open('MANIFEST.MF', 'w+') as fp:
        fp.write('Plugin-SymbolicName: com.atomdata.mcs.{}\n'.format(pluginName.lower()))
        fp.write('Require-Plugin: ')


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print('python create_plugin.py pluginName')
        sys.exit(1)

    pluginName = sys.argv[1]
    if os.path.isdir(pluginName):
        print('{} exists'.format(pluginName))
        sys.exit(1)

    os.mkdir(pluginName)
    os.chdir(pluginName)

    createPro(pluginName)
    createActivatorH(pluginName)
    createActivatorCpp(pluginName)
    createQrc(pluginName)
    createManifest(pluginName)




