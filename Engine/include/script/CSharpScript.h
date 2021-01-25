#pragma once

#include <string>

#include <core/Core.h>
#include <script/mono/mono_assembly.h>
#include <script/mono/mono_domain.h>

namespace Engine
{

class CSharpScript
{
public:
    static Shared<CSharpScript> create(const std::string& filepath, const mono::domain& domain);

    void onStart();
    void onUpdate(float dt);
    void onDestroy();

private:
    CSharpScript(const std::string& filepath, const mono::domain& domain);

private:
    mono::assembly m_assembly;
};

}