#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>

#include <renderer/Texture2D.h>
#include <core/Console.h>

class IAssetList
{
    friend class Assets;

protected:
    IAssetList() {}
    virtual ~IAssetList() = default;

    virtual int getAssetCount() = 0;
};

template <typename T>
class AssetList : public IAssetList
{
    friend class Assets;

public:
    void add(const std::string& key, Shared<T> asset)
    {
        m_assets.insert(std::make_pair(key, asset));
    }

    Shared<T> get(const std::string& key)
    {
        if (!exists(key))
        {
            Console::errf("Asset does not exist: %s", key);
        }

        return m_assets.at(key);
    }

    void remove(const std::string& key)
    {
        m_assets.erase(key);
    }

    bool exists(const std::string& key)
    {
        return m_assets.find(key) != m_assets.end();
    }

    int getAssetCount() override
    {
        return m_assets.size();
    }

private:
    std::unordered_map<std::string, Shared<T>> m_assets;
};

class Assets
{
public:
    Assets() {}

    ~Assets()
    {
        flush();
    }

    template<typename T>
    static void add(const std::string& key, Shared<T> asset)
    {
        if (m_instance.m_lists.find(typeid(T)) == m_instance.m_lists.end())
        {
            m_instance.m_lists.insert(std::pair<std::type_index, IAssetList*>(typeid(T), new AssetList<T>()));
        }

        getList<T>()->add(key, asset);
    }

    template<typename T>
    static Shared<T> get(const std::string& key)
    {
        if (!listExists<T>())
        {
            Console::err("Could not retrieve asset (check key).");
        }

        return getList<T>()->get(key);
    }

    template<typename T>
    static bool exists(const std::string& key)
    {
        if (!listExists<T>())
        {
            return false;
        }

        return getList<T>()->exists(key);
    }

    template<typename T>
    static void remove(const std::string& key)
    {
        if (!listExists<T>())
        {
            return;
        }

        getList<T>()->remove();
    }

    static void flush()
    {
        for (auto list : m_instance.m_lists)
        {
            delete list.second;
        }
        m_instance.m_lists.clear();
    }

private:
    std::unordered_map<std::type_index, IAssetList*> m_lists;

    template<typename T>
    static AssetList<T>* getList()
    {
        return (AssetList<T>*)m_instance.m_lists[typeid(T)];
    }

    template<typename T>
    static bool listExists()
    {
        return m_instance.m_lists.find(typeid(T)) != m_instance.m_lists.end();
    }

    static Assets m_instance;
};