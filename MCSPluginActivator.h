#ifndef MCSPLUGINACTIVATOR_H
#define MCSPLUGINACTIVATOR_H

#include <QObject>

class MCSPluginContext;

class MCSPluginActivator
{
public:
    virtual ~MCSPluginActivator() {}

    virtual void start(MCSPluginContext *context) = 0;

    virtual void stop(MCSPluginContext *context) = 0;
};


Q_DECLARE_INTERFACE(MCSPluginActivator, "com.atomdata.mcs.activator")


#endif // MCSPLUGINACTIVATOR_H
