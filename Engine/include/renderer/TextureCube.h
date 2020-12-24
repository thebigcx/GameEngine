#pragma once

#include <string>

#include <core/Core.h>

namespace Engine
{

class TextureCube
{
public:
    virtual ~TextureCube() = default;

    virtual void bind(uint32_t slot = 0) const = 0;
    virtual void unbind(uint32_t slot = 0) const = 0;
    
    virtual bool operator==(const TextureCube& array) const = 0;
    virtual bool operator!=(const TextureCube& array) const = 0;

    static Shared<TextureCube> create(const std::string& filepath);
    static Shared<TextureCube> create(const std::string* files);
};

}