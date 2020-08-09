#ifndef MCSPLUGINFRAMEWORK_H
#define MCSPLUGINFRAMEWORK_H

#include "mcs_global.h"

class MCSPluginFrameworkPrivate;

// 插件框架类
class MCSSHARED_EXPORT MCSPluginFramework
{
    Q_DISABLE_COPY(MCSPluginFramework)
public:
    MCSPluginFramework();
    ~MCSPluginFramework();

    // 安装一个插件
    bool installPlugin(const QString &filePath);

    // 开始
    void start();

    // 停止
    void stop();

private:
    MCSPluginFrameworkPrivate *d_ptr;
    Q_DECLARE_PRIVATE(MCSPluginFramework)
};

#endif // MCSPLUGINFRAMEWORK_H
