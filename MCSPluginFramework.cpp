#include "MCSPluginFramework.h"

#include <QPluginLoader>
#include <QFileInfo>
#include <QRegExp>
#include <QSet>
#include <QMap>
#include <QStringList>
#include <QDebug>

#include "MCSPluginContext.h"
#include "MCSPluginActivator.h"

class MCSPluginFrameworkPrivate
{
    struct PluginManifest
    {
        QString filePath;
        QString symbolicName;
        QSet<QString> requireSet;
    };
public:
    MCSPluginFrameworkPrivate()
        :context(new MCSPluginContext)
    {
    }

    ~MCSPluginFrameworkPrivate()
    {
        stop();
        delete context;
    }

    // 安装一个插件
    bool installPlugin(const QString &filePath)
    {
        // 从资源文件中读取插件符号名和插件依赖关系
        QFileInfo info(filePath);
        QPluginLoader loader(info.absoluteFilePath());
        if (!loader.load())
        {
            qCritical() <<  loader.errorString();
            return false;
        }

        QString baseName = info.completeBaseName();
        QFile file;
        if (baseName.startsWith("lib"))
        {
            file.setFileName(":/" + baseName.mid(3) + "/META-INF/MANIFEST.MF");
        }
        else
        {
            file.setFileName(":/" + baseName + "/META-INF/MANIFEST.MF");
        }


        if (!file.open(QFile::ReadOnly))
        {
            return false;
        }

        QHash<QString, QString> kv;
        QRegExp re("(.+):(.+)");
        while (!file.atEnd())
        {
            QByteArray line = file.readLine().trimmed();
            if (re.exactMatch(line))
            {
                kv[re.cap(1).trimmed()] = re.cap(2).trimmed();
            }
        }
        file.close();

        QString symbolicName = kv.value("Plugin-SymbolicName");
        if (symbolicName.isEmpty())
        {
            return false;
        }
        QStringList requireList;
        if (kv.contains("Require-Plugin"))
        {
            requireList = kv.value("Require-Plugin").split(',');
            std::transform(requireList.begin(), requireList.end(), requireList.begin(),
                           [](const QString &require){ return require.trimmed(); });
            requireList.erase(std::remove(requireList.begin(), requireList.end(), ""), requireList.end());
        }

#if QT_DEPRECATED_SINCE(5, 14) && QT_VERSION < QT_VERSION_CHECK(6,0,0)
        pluginManifests.append({info.absoluteFilePath(), symbolicName, QSet<QString>(requireList.begin(), requireList.end())});
#else
        pluginManifests.append({info.absoluteFilePath(), symbolicName, requireList.toSet()});
#endif
        return true;
    }

    void start()
    {
        // 根据依赖关系对插件进行排序
        auto requireMap = getRequireMap();
        auto sortValueMap = getSortValueMap(requireMap);
        std::sort(pluginManifests.begin(), pluginManifests.end(),
                  [sortValueMap](const PluginManifest &lhs, const PluginManifest &rhs)
        {
            int leftValue = sortValueMap.value(lhs.symbolicName, 0);
            int rightValue = sortValueMap.value(rhs.symbolicName, 0);
            return leftValue < rightValue;
        });

        // 启动
        for (const PluginManifest &manifest : pluginManifests)
        {
            QPluginLoader loader(manifest.filePath);
            if (!loader.isLoaded())
            {
                continue;
            }
            MCSPluginActivator *activator = qobject_cast<MCSPluginActivator *>(loader.instance());
            if (activator == nullptr)
            {
                continue;
            }
            activator->start(context);
        }
    }

    void stop()
    {
        for (const PluginManifest &manifest : pluginManifests)
        {
            QPluginLoader loader(manifest.filePath);
            if (!loader.isLoaded())
            {
                continue;
            }
            MCSPluginActivator *activator = qobject_cast<MCSPluginActivator *>(loader.instance());
            if (activator == nullptr)
            {
                continue;
            }
            activator->stop(context);
            loader.unload();
        }
    }

    inline MCSPluginContext *getContext() const
    {
        return context;
    }
private:
    QMap<QString, QSet<QString>> getRequireMap() const
    {
        QMap<QString, QSet<QString>> requireMap;
        for (const PluginManifest &manifest: pluginManifests)
        {
            if (requireMap.contains(manifest.symbolicName))
            {
                continue;
            }
            QSet<QString> requireSet = manifest.requireSet;

            for (auto itSet = requireSet.begin(); itSet != requireSet.end();)
            {
                if (manifest.symbolicName == *itSet)
                {
                    itSet = requireSet.erase(itSet);
                }
                else
                {
                    auto it = requireMap.find(*itSet);
                    if (it != requireMap.end())
                    {
                        if (it.value().contains(manifest.symbolicName))
                        {
                            itSet = requireSet.erase(itSet);
                            continue;
                        }
                    }
                    ++itSet;
                }
            }
            if (!requireSet.isEmpty())
            {
                requireMap[manifest.symbolicName] = requireSet;
            }
        }
        return requireMap;
    }

    QMap<QString, int> getSortValueMap(QMap<QString, QSet<QString>> &requireMap)
    {
        QMap<QString, int> sortValueMap;
        int value = 0;
        forever
        {

#if QT_DEPRECATED_SINCE(5, 14) && QT_VERSION < QT_VERSION_CHECK(6,0,0)
            QList<QString> keys = requireMap.keys();
            QSet<QString> leftSet(keys.begin(), keys.end());
#else
            QSet<QString> leftSet = requireMap.keys().toSet();
#endif
            QSet<QString> rightSet;
            for(const QSet<QString> &requireSet: requireMap.values())
            {
                rightSet |= requireSet;
            }

            QSet<QString> subSet = rightSet - leftSet;
            if (subSet.isEmpty())
            {
                break;
            }

            for (const QString &symbolicName: subSet)
            {
                sortValueMap[symbolicName] = value;
            }

            value++;

            for (auto it = requireMap.begin(); it != requireMap.end(); )
            {
                QSet<QString> &requireSet = it.value();
                requireSet -= subSet;
                if (requireSet.isEmpty())
                {
                    sortValueMap[it.key()] = value;
                    it = requireMap.erase(it);
                }
                else
                {
                    ++it;
                }
            }
            value++;
        }
        return sortValueMap;
    }

private:
    MCSPluginContext *context;
    QList<PluginManifest> pluginManifests;

};


MCSPluginFramework::MCSPluginFramework()
    :d_ptr(new MCSPluginFrameworkPrivate)
{

}

MCSPluginFramework::~MCSPluginFramework()
{
    delete d_ptr;
}

bool MCSPluginFramework::installPlugin(const QString &filePath)
{
    Q_D(MCSPluginFramework);
    return d->installPlugin(filePath);
}

void MCSPluginFramework::start()
{
    Q_D(MCSPluginFramework);
    d->start();
}

void MCSPluginFramework::stop()
{
    Q_D(MCSPluginFramework);
    d->stop();
}

MCSPluginContext *MCSPluginFramework::getContext() const
{
    Q_D(const MCSPluginFramework);
    return d->getContext();
}
