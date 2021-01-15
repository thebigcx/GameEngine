#pragma once

#include <mono/jit/jit.h>

namespace Engine
{
namespace mono
{

class image
{
public:
    image() = default;
    image(MonoImage* img)
        : m_image(img) {}

    MonoImage* get() const { return m_image; }

private:
    MonoImage* m_image = nullptr;
};

}
}