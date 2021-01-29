#include <script/ScriptController.h>
#include <scene/Components.h>
#include <scene/Scene.h>
#include <util/io/FileSystem.h>

namespace Engine
{

ScriptController::ScriptController()
{
    
}

ScriptController::~ScriptController()
{

}

ScriptController* ScriptController::getInstance()
{
    static ScriptController controller;
    return &controller;
}

Shared<CSharpScript> ScriptController::loadScript(const std::string& filepath)
{
    auto script = CSharpScript::create(filepath, m_domain);
    m_scripts.push_back(script);

    return script;
}

void ScriptController::unloadScript(const Shared<CSharpScript>& script)
{
    for (std::vector<Shared<CSharpScript>>::iterator iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
    {
        if (*iter == script)
        {
            m_scripts.erase(iter);
            break;
        }
    }
}

void ScriptController::initialize()
{
    Mono::init("mono");
    m_domain.create("Engine");
    Engine::Mono::Domain::setCurrentDomain(m_domain);

    auto assembly = m_domain.getAssembly("Engine.dll");
    auto type = assembly.getType("Example", "Dog");
    auto object = type.createInstance();
    auto method = type.getMethod("bark");

    auto thunk = Engine::Mono::makeMethodInvoker<void(void)>(method);
    thunk(object);
    thunk(object);

    auto prop = type.getProperty("Barks");
    auto propInvoker = Mono::makePropertyInvoker<int>(prop);

    std::cout << propInvoker.getValue(object) << "\n";
}

void ScriptController::finalize()
{
    Mono::shutdown();
}

void ScriptController::onEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MousePressedEvent>(BIND_EVENT_FN(ScriptController::onMousePressed));
}

bool ScriptController::onMousePressed(MousePressedEvent& event)
{
    
    return false;
}

bool ScriptController::onKeyPressed(KeyPressedEvent& event)
{
    
    return false;
}

}