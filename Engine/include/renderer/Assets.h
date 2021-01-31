#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include <core/Logger.h>
#include <core/Core.h>
#include <util/io/Deserializer.h>
#include <renderer/Texture2D.h>
#include <renderer/shader/Shader.h>
#include <util/uuid.h>

namespace Engine
{
// TODO: refactor this part of the engine (resource management)
// TODO: could be good to preload all assets inside a certain folder

template<typename T>
struct AssetLoader;

template<>
struct AssetLoader<Texture2D>
{
    static Reference<Texture2D> load(const std::string& path)
    {
        return Deserializer::loadTexture(path);
    }
};

template<>
struct AssetLoader<Shader>
{
    static Reference<Shader> load(const std::string& path)
    {
        return Deserializer::loadShader(path);
    }
};

template<>
struct AssetLoader<Material>
{
    static Reference<Material> load(const std::string& path)
    {
        return Deserializer::loadMaterial(path);
    }
};

template<>
struct AssetLoader<Mesh>
{
    static Reference<Mesh> load(const std::string& path)
    {
        return Deserializer::loadMesh(path);
    }
};

class IAssetCache
{
    friend class Assets;

protected:
    IAssetCache() {}
    virtual ~IAssetCache() = default;

    virtual const int getAssetCount() noexcept = 0;
};

template <typename T>
class AssetCache : public IAssetCache
{
    friend class Assets;

public:
    void add(const std::string& key, const Reference<T>& asset);

    Weak<T> get(const std::string& key);

    void remove(const std::string& key);

    const bool exists(const std::string& key);

    const int getAssetCount() noexcept override;

    typename std::unordered_map<std::string, Reference<T>>::iterator begin() { return m_assets.begin(); }
    typename std::unordered_map<std::string, Reference<T>>::iterator end()   { return m_assets.end(); }
    typename std::unordered_map<std::string, Reference<T>>::const_iterator begin() const { return m_assets.begin(); }
    typename std::unordered_map<std::string, Reference<T>>::const_iterator end()   const { return m_assets.end(); }

    std::unordered_map<std::string, Reference<T>>& getInternalList();

    void load(const std::string& path)
    {
        m_assets.insert(std::make_pair(Utils::genUUID(), AssetLoader<T>::load(path)));
    }

private:
    std::unordered_map<std::string, Reference<T>> m_assets;
};

// -------------------------------------------------------------------------------------------

template<typename T>
void AssetCache<T>::add(const std::string& key, const Reference<T>& asset)
{
    m_assets.insert(std::make_pair(key, asset));
}

template<typename T>
Weak<T> AssetCache<T>::get(const std::string& key)
{
    if (!exists(key))
    {
        Reference<T> ref(nullptr);
        return ref;
    }

    return m_assets.at(key);
}

template<typename T>
void AssetCache<T>::remove(const std::string& key)
{
    if (this->exists(key))
    {
        m_assets.erase(key);
    }
}

template<typename T>
const bool AssetCache<T>::exists(const std::string& key)
{
    return m_assets.find(key) != m_assets.end();
}

template<typename T>
const int AssetCache<T>::getAssetCount() noexcept
{
    return m_assets.size();
}

template<typename T>
std::unordered_map<std::string, Reference<T>>& AssetCache<T>::getInternalList()
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
        if (!cacheExists<T>())
        {
            m_instance->m_caches.insert(std::pair<std::type_index, IAssetCache*>(typeid(T), new AssetCache<T>()));
        }

        getCache<T>().add(key, asset);
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
    static Weak<T> get(const std::string& key)
    {
        if (!cacheExists<T>())
        {
            m_instance->m_caches.insert(std::pair<std::type_index, IAssetCache*>(typeid(T), new AssetCache<T>()));
        }

        return getCache<T>().get(key);
    }

    template<typename T>
    static bool exists(const std::string& key)
    {
        if (!cacheExists<T>())
        {
            return false;
        }

        return getCache<T>().exists(key);
    }

    template<typename T>
    static void remove(const std::string& key)
    {
        if (!cacheExists<T>())
        {
            return;
        }

        getCache<T>().remove(key);
    }

    static void flush();

    template<typename T>
    static unsigned int getAssetCount() noexcept
    {
        if (!cacheExists<T>())
        {
            return 0;
        }

        return getCache<T>().getAssetCount();
    }

    template<typename T>
    static AssetCache<T>& getCache()
    {
        if (!cacheExists<T>())
        {
            m_instance->m_caches.emplace(std::make_pair(static_cast<std::type_index>(typeid(T)), new AssetCache<T>()));
        }

        return *static_cast<AssetCache<T>*>(m_instance->m_caches.at(typeid(T)));
    }

    template<typename T>
    static bool cacheExists()
    {
        return m_instance->m_caches.find(typeid(T)) != m_instance->m_caches.end();
    }

    template<typename T>
    static const std::string& find(const Reference<T>& assetToFind)
    {
        for (auto& asset : getCache<T>().getInternalList())
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
    std::unordered_map<std::type_index, IAssetCache*> m_caches;

    static Assets* m_instance;
};

}