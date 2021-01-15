#pragma once

#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <script/mono/mono_image.h>
#include <script/mono/mono_type.h>

namespace Engine
{
namespace mono
{

class assembly
{
public:
    assembly() = default;
    assembly(MonoAssembly* assem)
        : m_assembly(assem) {}

    image get_image();
    type get_type(const std::string& namespace_, const std::string& name);

    MonoAssembly* get() const { return m_assembly; }

private:
    MonoAssembly* m_assembly = nullptr;
};

}
}