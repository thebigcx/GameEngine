#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <vector>

#include <renderer/Texture2D.h>
#include <core/Logger.h>

namespace Engine
{
// TODO: refactor this part of the engine (resource management)
class IAssetList
{
    friend class Assets;

protected:
    IAssetList() {}
    virtual ~IAssetList() = default;

    virtual const int getAssetCount() noexcept = 0;
};

template <typename T>
class AssetList : public IAssetList
{
    friend class Assets;

public:
    inline void add(const std::string& key, const Shared<T>& asset)
    {
        m_assets.insert(std::make_pair(key, asset));
    }

    const Shared<T>& get(const std::string& key)
    {
        if (!exists(key))
        {
            Logger::getCoreLogger()->error("Asset does not exist: %s", key.c_str());
            abort();

            static Shared<T> fail = nullptr;
            return fail;
        }

        return m_assets.at(key);
    }

    inline void remove(const std::string& key)
    {
        if (this->exists(key))
        {
            m_assets.erase(key);
        }
    }

    inline constexpr const bool exists(const std::string& key)
    {
        return m_assets.find(key) != m_assets.end();
    }

    inline const int getAssetCount() noexcept override
    {
        return m_assets.size();
    }

    typename std::unordered_map<std::string, Shared<T>>::iterator begin() { return m_assets.begin(); }
    typename std::unordered_map<std::string, Shared<T>>::iterator end()   { return m_assets.end(); }
    typename std::unordered_map<std::string, Shared<T>>::const_iterator begin() const { return m_assets.begin(); }
    typename std::unordered_map<std::string, Shared<T>>::const_iterator end()   const { return m_assets.end(); }

    constexpr std::unordered_map<std::string, Shared<T>>& getInternalList()
    {
        return m_assets;
    }

private:
    std::unordered_map<std::string, Shared<T>> m_assets;
};
// TODO: add callback system for when an asset is deleted. this notifies all users of the particular asset to switch to something else
// a 'null' asset would work very well here, as when an asset is deleted, the null asset is the fallback.
class Assets
{
public:
    Assets() = default;

    ~Assets()
    {
        flush();
    }

    template<typename T>
    static inline void add(const std::string& key, Shared<T> asset)
    {
        if (!listExists<T>())
        {
            m_instance.m_lists.insert(std::pair<std::type_index, IAssetList*>(typeid(T), new AssetList<T>()));
        }

        getList<T>().add(key, asset);
    }

    template<typename T>
    static inline void addIfNotExists(const std::string& key, Shared<T> asset)
    {
        if (find(asset) == "")
        {
            add(key, asset);
        }
    }

    template<typename T>
    static inline const Shared<T>& get(const std::string& key)
    {
        if (!listExists<T>())
        {
            Logger::getCoreLogger()->error("Asset List does not exist.");

            static Shared<T> fail = nullptr;
            return fail;
        }

        return getList<T>().get(key);
    }

    template<typename T>
    static inline bool exists(const std::string& key)
    {
        if (!listExists<T>())
        {
            return false;
        }

        return getList<T>().exists(key);
    }

    template<typename T>
    static inline void remove(const std::string& key)
    {
        if (!listExists<T>())
        {
            return;
        }

        getList<T>().remove(key);
    }

    static inline void flush()
    {
        for (auto& list : m_instance.m_lists)
        {
            delete list.second;
        }

        m_instance.m_lists.clear();
    }

    template<typename T>
    static inline unsigned int getAssetCount() noexcept
    {
        if (!listExists<T>())
        {
            return 0;
        }

        return getList<T>().getAssetCount();
    }

    template<typename T>
    static inline constexpr AssetList<T>& getList()
    {
        return *static_cast<AssetList<T>*>(m_instance.m_lists.at(typeid(T)));
    }

    template<typename T>
    static inline constexpr bool listExists()
    {
        return m_instance.m_lists.find(typeid(T)) != m_instance.m_lists.end();
    }

    template<typename T>
    static inline const std::string& find(const Shared<T>& assetToFind)
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

    template<typename T>
    static inline std::string generateID()
    {
        int candidate = getAssetCount<T>();
        while (getList<T>().getInternalList().find(std::to_string(candidate)) != getList<T>().getInternalList().end())
        {
            candidate++;
        }

        return std::to_string(candidate);
    }

private:
    std::unordered_map<std::type_index, IAssetList*> m_lists;

    static Assets m_instance;
};

}