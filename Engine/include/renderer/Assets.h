#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include <core/Logger.h>
#include <core/Core.h>

namespace Engine
{
// TODO: refactor this part of the engine (resource management)
// TODO: could be good to preload all assets inside a certain folder
class IAssetPool
{
    friend class Assets;

protected:
    IAssetPool() {}
    virtual ~IAssetPool() = default;

    virtual const int getAssetCount() noexcept = 0;
};

template <typename T>
class AssetPool : public IAssetPool
{
    friend class Assets;

public:
    void add(const std::string& key, const Reference<T>& asset);

    const Reference<T>& get(const std::string& key);

    void remove(const std::string& key);

    const bool exists(const std::string& key);

    const int getAssetCount() noexcept override;

    typename std::unordered_map<std::string, Reference<T>>::iterator begin() { return m_assets.begin(); }
    typename std::unordered_map<std::string, Reference<T>>::iterator end()   { return m_assets.end(); }
    typename std::unordered_map<std::string, Reference<T>>::const_iterator begin() const { return m_assets.begin(); }
    typename std::unordered_map<std::string, Reference<T>>::const_iterator end()   const { return m_assets.end(); }

    std::unordered_map<std::string, Reference<T>>& getInternalList();

private:
    std::unordered_map<std::string, Reference<T>> m_assets;
};

// -------------------------------------------------------------------------------------------

template<typename T>
void AssetPool<T>::add(const std::string& key, const Reference<T>& asset)
{
    m_assets.insert(std::make_pair(key, asset));
}

template<typename T>
const Reference<T>& AssetPool<T>::get(const std::string& key)
{
    if (!exists(key))
    {
        Logger::getCoreLogger()->error("Asset does not exist: %s", key.c_str());
        abort();

        static Reference<T> fail = nullptr;
        return fail;
    }

    return m_assets.at(key);
}

template<typename T>
void AssetPool<T>::remove(const std::string& key)
{
    if (this->exists(key))
    {
        m_assets.erase(key);
    }
}

template<typename T>
const bool AssetPool<T>::exists(const std::string& key)
{
    return m_assets.find(key) != m_assets.end();
}

template<typename T>
const int AssetPool<T>::getAssetCount() noexcept
{
    return m_assets.size();
}

template<typename T>
std::unordered_map<std::string, Reference<T>>& AssetPool<T>::getInternalList()
{
    return m_assets;
}

// -------------------------------------------------------------------------------------------

// TODO: add callback system for when an asset is deleted. this notifies all users of the particular asset to switch to something else
// a 'null' asset would work very well here, as when an asset is deleted, the null asset is the fallback.
// TODO: Add UUIDs for key uniqueness.
class Assets
{
public:
    Assets();
    ~Assets();

    template<typename T>
    static void add(const std::string& key, Reference<T> asset)
    {
        if (!listExists<T>())
        {
            m_instance->m_lists.insert(std::pair<std::type_index, IAssetPool*>(typeid(T), new AssetPool<T>()));
        }

        getList<T>().add(key, asset);
    }

    template<typename T>
    static void addIfNotExists(const std::string& key, Reference<T> asset)
    {
        if (find(asset) == "")
        {
            add(key, asset);
        }
    }

    template<typename T>
    static const Reference<T>& get(const std::string& key)
    {
        if (!listExists<T>())
        {
            Logger::getCoreLogger()->error("Asset List does not exist.");

            static Reference<T> fail = nullptr;
            return fail;
        }

        return getList<T>().get(key);
    }

    template<typename T>
    static bool exists(const std::string& key)
    {
        if (!listExists<T>())
        {
            return false;
        }

        return getList<T>().exists(key);
    }

    template<typename T>
    static void remove(const std::string& key)
    {
        if (!listExists<T>())
        {
            return;
        }

        getList<T>().remove(key);
    }

    static void flush();

    template<typename T>
    static unsigned int getAssetCount() noexcept
    {
        if (!listExists<T>())
        {
            return 0;
        }

        return getList<T>().getAssetCount();
    }

    template<typename T>
    static AssetPool<T>& getList()
    {
        if (!listExists<T>())
        {
            m_instance->m_lists.emplace(std::make_pair(static_cast<std::type_index>(typeid(T)), new AssetPool<T>()));
        }

        return *static_cast<AssetPool<T>*>(m_instance->m_lists.at(typeid(T)));
    }

    template<typename T>
    static bool listExists()
    {
        return m_instance->m_lists.find(typeid(T)) != m_instance->m_lists.end();
    }

    template<typename T>
    static const std::string& find(const Reference<T>& assetToFind)
    {
        for (auto& asset : getList<T>().getInternalList())
        {
            if (asset.second == assetToFind)
            {
                return asset.first;
            }
        }

        static std::string fail = "";
        return fail;
    }

private:
    std::unordered_map<std::type_index, IAssetPool*> m_lists;

    static Assets* m_instance;
};

}