#pragma once

#include <string>

#include <core/Core.h>
#include <Mono/Assembly.h>
#include <Mono/Domain.h>
#include <scene/GameComponent.h>

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

class ScriptInstance : public GameComponent
{
public:
    void setScript(const Reference<Script>& script) { m_script = script; };
    
    const Reference<Script> getScript() const { return m_script; }

private:
    Reference<Script> m_script = nullptr;
};

}