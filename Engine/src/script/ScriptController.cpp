#include <script/ScriptController.h>
#include <scene/Components.h>
#include <scene/Scene.h>
#include <util/io/FileSystem.h>

#include <events/Event.h>
#include <events/EventDispatcher.h>
#include <events/MouseEvent.h>
#include <events/KeyboardEvent.h>
#include <script/mono/Field.h>

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

void ScriptController::recompileScripts()
{
    std::string command = "csc -out:Game.dll -target:library Editor/scripts/Engine.cs ";
    for (auto& script : m_scripts)
    {
        command += script->getPath() + " ";
    }

    std::system(command.c_str());

    m_assembly = m_domain.getAssembly("Game.dll");

    m_engineTypes.insert(std::make_pair("CameraComponent", m_assembly.getType("Engine", "CameraComponent").get()));
    m_engineTypes.insert(std::make_pair("TransformComponent", m_assembly.getType("Engine", "TransformComponent").get()));
}

Reference<Script> ScriptController::loadScript(const std::string& filepath)
{
    auto script = Script::create(filepath, m_domain);
    m_scripts.push_back(script);

    recompileScripts();

    auto type = m_assembly.getType("", "CameraController");
    auto object = type.createInstance();

    auto prop = type.getProperty("TranslationX");
    auto propInvoker = Mono::makePropertyInvoker<float>(prop);
    std::cout << propInvoker.getValue(object) << "\n";

    return script;
}

void ScriptController::unloadScript(const Reference<Script>& script)
{
    for (std::vector<Reference<Script>>::iterator iter = m_scripts.begin(); iter != m_scripts.end(); iter++)
    {
        if (*iter == script)
        {
            m_scripts.erase(iter);
            break;
        }
    }
    
    recompileScripts();
}

static void getCameraComponent(uint32_t gameObject, MonoObject** component)
{
    auto temp = new CameraComponent();
    
    

    component = &object.get();
}

static void getTransformComponent(uint32_t gameObject, MonoObject** component)
{
    auto temp = new TransformComponent();
    
    Mono::Object obj = ScriptController::getInstance()->m_engineTypes["TransformComponent"].createInstance();
    

    component = &object.get();
}

void ScriptController::initialize()
{
    Mono::init("mono");
    m_domain.create("Engine");
    Engine::Mono::Domain::setCurrentDomain(m_domain);

    //recompileScripts();

    

    mono_add_internal_call("Engine.NativeBinding::getCameraComponent_Native", (const void*)getCameraComponent);
    mono_add_internal_call("Engine.NativeBinding::getTransformComponent_Native", (const void*)getTransformComponent);

    /*auto assembly = m_domain.getAssembly("Engine.dll");
    auto type = assembly.getType("Example", "Dog");
    auto object = type.createInstance();
    auto method = type.getMethod("bark");

    auto thunk = Engine::Mono::makeMethodInvoker<void(void)>(method);
    thunk(object);
    thunk(object);

    auto prop = type.getProperty("Barks");
    auto propInvoker = Mono::makePropertyInvoker<int>(prop);

    std::cout << propInvoker.getValue(object) << "\n";*/
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