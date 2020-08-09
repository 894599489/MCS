#include "MCSPluginContext.h"
#include <QHash>

class MCSPluginContextPrivate
{
public:
    QHash<QString, QObject *> services;
};


MCSPluginContext::MCSPluginContext()
    :d_ptr(new MCSPluginContextPrivate)
{

}

MCSPluginContext::~MCSPluginContext()
{
    delete d_ptr;
}

bool MCSPluginContext::registerService(const char *clazz, QObject *service)
{
    Q_D(MCSPluginContext);
    QString key(clazz);
    if (d->services.contains(key))
    {
        return false;
    }
    d->services[key] = service;
    return true;
}

QObject *MCSPluginContext::getService(const char *clazz) const
{
    Q_D(const MCSPluginContext);
    return d->services.value(clazz);
}

