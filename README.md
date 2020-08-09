# 介绍
  MCS是基于Qt5的一个轻量级插件框架，整个框架面向服务接口编程。框架特点是可以指定插件之间的依赖关系。

# 核心部件
    * MCSPluginFramework
	框架操作类，提供installPlugin、start、stop方法
  
	* MCSPluginContext
	上下文管理类，提供注册、发现服务方法
  
	* MCSPluginActivator
	生命周期接口，每一个插件需要继承该接口，用于通知插件的开始和停止
  
# Demo
1.插件
	- 定义一个服务接口AInterface
	```cpp
	class AInterface
	{
	public:
	  virtual ~AInterface() {}

	  virtual int getAValue() const = 0;
	};

	Q_DECLARE_INTERFACE(AInterface, "com.atomdata.mcs.service.a")
	```
  
	- 实现服务接口AInterface
	```cpp
	class AService : public QObject, public AInterface
	{
	  Q_OBJECT
	  Q_INTERFACES(AInterface)
	public:
	  explicit AService(QObject *parent = nullptr);
	  virtual int getAValue() const override {return 100;}
	};
	```
  
	- 注册服务
	```cpp
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
  
	- A.pro内容
	必须包含一个资源文件 RESOURCES += res.qrc, 添加一个前缀 ***/com.atomdata.mcs.a/META-INF*** ，格式必须是 ***/插件生成的名字/META-INF***
	在资源文件中添加一个清单文件 ***MANIFEST.MF*** ，内容如下：
		Plugin-SymbolicName : com.atomdata.mcs.a #符号名字
		Require-Plugin: #依赖其他哪些插件，以,分割
  
	- 在插件B中调用A
	```cpp
	class BActivator : public QObject, public MCSPluginActivator
	{
	  Q_OBJECT
	  Q_INTERFACES(MCSPluginActivator)
	  Q_PLUGIN_METADATA(IID "com.atomdata.mcs.b")
	public:
	  virtual void start(MCSPluginContext *context) override
	  {
		auto aintf = context->getService<AInterface>();
		int v = aintf->getAValue();
		qDebug() << "AInterface::getAValue " << v;
	  }
	  virtual void stop(MCSPluginContext *context) override;
	};
	```
  
	- B的 ***MANIFEST.MF*** 内容
    Plugin-SymbolicName: com.atomdata.mcs.b
    Require-Plugin: com.atomdata.mcs.a #依赖于A

  
2.main函数
  ```cpp
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
  
