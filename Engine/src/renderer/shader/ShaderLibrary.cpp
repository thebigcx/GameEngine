#include <renderer/shader/ShaderLibrary.h>
#include <renderer/Assets.h>

namespace Engine
{

void ShaderLibrary::setupShaders()
{
    auto addShader = [](const std::string& file, const std::string& name)
    {
        auto shader = Shader::createFromFile(file);
        shader->name = name;
        shader->uuid = Utils::genUUID();
        Assets::add<Shader>(shader->name, shader);
    };

    addShader("Engine/assets/shaders/EnginePBR_Static.glsl", "EnginePBR_Static");
    //addShader("Engine/assets/shaders/EnginePBR_Instanced.glsl", "EnginePBR_Instanced");
    addShader("Engine/assets/shaders/EngineHDR_Pass.glsl", "EngineHDR_Pass");
    addShader("Engine/assets/shaders/EngineFX_Outline.glsl", "EngineFX_Outline");
    addShader("Engine/assets/shaders/EngineShadow_Map.glsl", "EngineShadow_Map");
    addShader("Engine/assets/shaders/EngineIBL_Environment.glsl", "EngineIBL_Environment");
    addShader("Engine/assets/shaders/Engine2D_Text.glsl", "Engine2D_Text");
    addShader("Engine/assets/shaders/Engine2D_Texture.glsl", "Engine2D_Texture");
}

}