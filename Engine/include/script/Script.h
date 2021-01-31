#pragma once

#include <string>

#include <core/Core.h>
#include <script/mono/Assembly.h>
#include <script/mono/Domain.h>

namespace Engine
{

class Script
{
public:
    static Reference<Script> create(const std::string& filepath, const Mono::Domain& domain);

    void onStart();
    void onUpdate(float dt);
    void onDestroy();

    inline const std::string& getPath() const { return m_path; }

private:
    Script(const std::string& filepath, const Mono::Domain& domain);

private:
    std::string m_path;
};

}