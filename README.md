# 介绍
  MCS是基于Qt5的一个轻量级插件框架，整个框架面向服务接口编程。框架特点是可以指定插件之间的依赖关系。

# 核心部件
* MCSPluginFramework
  框架操作类，提供installPlugin、start、stop方法
  
* MCSPluginContext
  上下文管理类，提供注册、发现服务方法
  
* MCSPluginActivator
  生命周期接口，每一个插件需要继承该接口，用于通知插件的开始和停止
  
#Demo
* 插件
  定义一个插件接口AInterface
  ```cpp
  class AInterface
  {
  public:
      virtual ~AInterface() {}

      virtual int getAValue() const = 0;
  };
  
  Q_DECLARE_INTERFACE(AInterface, "com.atomdata.mcs.service.a")
  ```
```cpp
#include "MCSPluginActivator.h"

class AActivator : public QObject, public MCSPluginActivator
{
    Q_OBJECT
    Q_INTERFACES(MCSPluginActivator)
    Q_PLUGIN_METADATA(IID "com.atomdata.mcs.a")
public:
    virtual void start(MCSPluginContext *context) override
    {
      context->registerService<AInterface>(new AService(this));
    }
    virtual void stop(MCSPluginContext *context) override {}
};
```

* main函数
```cpp
#include <QCoreApplication>

#include "MCSPluginFramework.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    MCSPluginFramework fw;
    // 安装一个插件，传入路径
    fw.installPlugin("plugins/com.atomdata.mcs.a.dll");
    // 开始
    fw.start();
    // 停止
    fw.stop();
    return a.exec();
}
```
  
