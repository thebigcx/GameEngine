#pragma once

#include <string>
#include <unordered_map>

#include <renderer/Texture2D.h>

template <typename T>
class AssetHolder
{
public:
    void add(const std::string& name, T& asset)
    {
        m_assets.insert(std::make_pair(name, std::move(asset)));
    }

    T& get(const std::string& name)
    {
        if (!exists(name))
        {
            Logger::errf("Asset does not exist: %s", name);
        }

        return m_assets.at(name);
    }

    void remove(const std::string& name)
    {
        m_assets.erase(name);
    }

    bool exists(const std::string& name)
    {
        return m_assets.find(name) != m_assets.end();
    }

private:
    std::unordered_map<std::string, T> m_assets;
};

class AssetManager
{
public:
    AssetManager()
    {
        
    }

    static AssetManager& get()
    {
        static AssetManager manager;
        return manager;
    }

    AssetHolder<Texture2D> textures;
};