#pragma once

#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <script/mono/mono_assembly.h>

namespace Engine
{
namespace mono
{

bool init(const std::string& dom);
void shutdown();

class domain
{
public:
    domain() = default;
    domain(const std::string& name);
    domain(MonoDomain* dom)
        : m_domain(dom) {}

    void create(const std::string& name);

    assembly get_assembly(const std::string& file);

    MonoDomain* get() const { return m_domain; }

    static void set_current_domain(const domain& dom);
    static inline const domain* get_current_domain() { return s_current_domain; }

private:
    MonoDomain* m_domain = nullptr;

    static inline const domain* s_current_domain = nullptr;
};

}
}