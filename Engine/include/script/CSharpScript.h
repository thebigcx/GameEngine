#pragma once

#include <string>

#include <core/Core.h>
#include <script/mono/Assembly.h>
#include <script/mono/Domain.h>

namespace Engine
{

class CSharpScript
{
public:
    static Shared<CSharpScript> create(const std::string& filepath, const Mono::Domain& domain);

    void onStart();
    void onUpdate(float dt);
    void onDestroy();

private:
    CSharpScript(const std::string& filepath, const Mono::Domain& domain);

private:
    Mono::Assembly m_assembly;
};

}