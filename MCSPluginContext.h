#ifndef MCSPLUGINCONTEXT_H
#define MCSPLUGINCONTEXT_H

#include "mcs_global.h"
#include <QObject>
class MCSPluginContextPrivate;

// 插件上下文管理类
class MCSSHARED_EXPORT MCSPluginContext
{
    Q_DISABLE_COPY(MCSPluginContext)
public:
    MCSPluginContext();
    ~MCSPluginContext();

    // 注册服务
    template <typename S>
    bool registerService(QObject *service)
    {
        const char *clazz = qobject_interface_iid<S *>();
        if (clazz == nullptr)
        {
            return false;
        }
        return registerService(clazz, service);
    }

    // 获取服务
    template <typename S>
    S *getService() const
    {
        const char *clazz = qobject_interface_iid<S *>();
        if (clazz == nullptr)
        {
            return nullptr;
        }

        return qobject_cast<S *>(getService(clazz));
    }

private:
    bool registerService(const char *clazz, QObject *service);
    QObject *getService(const char *clazz) const;

private:
    MCSPluginContextPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MCSPluginContext)
};

#endif // MCSPLUGINCONTEXT_H
