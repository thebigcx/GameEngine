#include "Sandbox.h"

Sandbox::Sandbox()
{
    
}

void Sandbox::onAttach()
{
    Engine::Application::get().getWindow().setSize(math::uvec2(1280, 720));

    Engine::Assets::add<Engine::Texture2D>("texture", Engine::Texture2D::create("Sandbox/assets/terrain.png", true));
    Engine::Assets::add<Engine::Texture2D>("grass", Engine::Texture2D::create("Sandbox/assets/grass.png", true));

    m_font = Engine::TrueTypeFont::create("Sandbox/assets/minecraftia.ttf", 48);

    Engine::RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));

    m_cubeMaterial = Engine::Material::create(Engine::Shader::createFromFile("Engine/src/renderer/shader/default/basic3d.glsl"));
    m_cubeMaterial->albedoMap = Engine::Assets::get<Engine::Texture2D>("grass");

    m_skyLight = Engine::SkyLight(math::vec3(1), 1.f);
    m_directionalLight = Engine::DirectionalLight(math::vec3(1), 1.f, math::vec3(0, -1, 0));
    m_pointLights.push_back(Engine::PointLight(math::vec3(1), 1.f, math::vec3(3, 0, 2)));

    Engine::Renderer3D::addLight(&m_skyLight);
    Engine::Renderer3D::addLight(&m_directionalLight);

    for (auto& pointLight : m_pointLights)
    {
        Engine::Renderer3D::addLight(&pointLight);
    }

    m_model = Engine::Model::load("Sandbox/assets/Donut.obj");

    m_mesh = Engine::MeshFactory::cubeMesh(1.f);
    m_mesh->material = m_cubeMaterial;
    //m_mesh->setVertices(std::vector<math::vec3>(), std::vector<math::vec3>(), std::vector<uint32_t>());
}

void Sandbox::onDetach()
{
    
}

void Sandbox::onUpdate(float dt)
{
    Engine::Timer timer;

    if (Engine::Input::isKeyPressed(Engine::Key::Escape))
    {
        Engine::Application::get().quit();
    }

    m_perspectiveCamera.update(dt);

    //Engine::RenderCommand::clear(Engine::RenderCommand::defaultClearBits());
    Engine::Renderer::startFrame();

    Engine::Renderer3D::beginScene(m_perspectiveCamera);

    Engine::Renderer3D::submit(m_model, math::translate(math::mat4(1.f), math::vec3(0, 0, 3)));

    for (unsigned int i = 0; i < 10; i++)
    for (unsigned int j = 0; j < 10; j++)
    {
        Engine::Renderer3D::submit(m_mesh, math::scale(math::translate(math::mat4(1.f), math::vec3(i * 2, j * 2, 0)), math::vec3(2.f)));
    }

    m_pointLights[0].position = m_perspectiveCamera.getPosition();

    Engine::Renderer3D::submit(m_mesh, math::translate(math::mat4(1.f), math::vec3(2.f, 1.f, 4.f)));

    Engine::Renderer3D::endScene();

    Engine::Renderer::endFrame();
}

void Sandbox::onEvent(Engine::Event& event)
{
    m_perspectiveCamera.onEvent(event);
}