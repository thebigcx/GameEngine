#include "Sandbox.h"

Sandbox::Sandbox()
{
    
}

void Sandbox::onAttach()
{
    Engine::Application::get().getWindow().setSize(math::uvec2(1280, 720));

    Engine::Assets::add<Engine::Texture2D>("texture", Engine::Texture2D::create("Sandbox/assets/terrain.png"));
    Engine::Assets::add<Engine::Texture2D>("grass", Engine::Texture2D::create("Sandbox/assets/grass.png"));

    m_font = Engine::TrueTypeFont::create("Sandbox/assets/minecraftia.ttf", 48);

    Engine::RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));

    m_orthoCamera.setPosition(math::vec2(0, 0));

    m_cubeMaterial = Engine::Material::create(Engine::Renderer3D::data.modelShader);
    m_cubeMaterial->albedoMap = Engine::Assets::get<Engine::Texture2D>("grass");
    m_cubeMaterial->metalness = 16.f;

    lights.setSkylight(0.05f);

    Engine::DirectionalLight dirLight;
    dirLight.direction = math::vec3(-0.6f, -0.6f, -0.6f);
    dirLight.intensity = 0.2f;
    dirLight.radiance = math::vec3(1, 1, 1);

    lights.setDirectionalLight(dirLight);

    std::vector<Engine::PointLight> pointLights;
    for (unsigned int i = 0; i < 1; i++)
    {
        Engine::PointLight pointLight;

        pointLight.radiance = math::vec3(1, 1, 1);
        pointLight.intensity = 1.f;
        pointLight.attenuation = 500.f;
        
        pointLights.push_back(pointLight);
    }

    lights.setPointLights(pointLights);

    std::vector<Engine::SpotLight> spotLights;
    for (unsigned int i = 0; i < 1; i++)
    {
        Engine::SpotLight spotLight;

        spotLight.radiance = math::vec3(1, 1, 1);
        spotLight.intensity = 1.f;
        spotLight.cutoff = math::cos(math::radians(12.5f));
        spotLight.outerCutoff = math::cos(math::radians(17.5f));
        
        spotLights.push_back(spotLight);
    }

    lights.setSpotLights(spotLights);

    Engine::Renderer3D::setLights(lights);

    m_model = Engine::Model::loadModel("Sandbox/assets/Donut.obj");

    m_mesh = Engine::MeshFactory::cubeMesh(1.f);
    m_mesh->material = m_cubeMaterial;
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

    Engine::RenderCommand::clear(Engine::RenderCommand::defaultClearBits());

    Engine::Renderer3D::beginScene(m_perspectiveCamera);

    Engine::Renderer3D::submit(m_model, math::translate(math::mat4(1.f), math::vec3(0, 0, 3)));

    for (unsigned int i = 0; i < 100; i++)
    for (unsigned int j = 0; j < 100; j++)
    {
        Engine::Renderer3D::submit(m_mesh, math::scale(math::translate(math::mat4(1.f), math::vec3(i * 2, j * 2, 0)), math::vec3(2.f)));
    }

    Engine::Renderer3D::data.modelShader->bind();
    Engine::Renderer3D::data.modelShader->setFloat3("pointLights[0].position", m_perspectiveCamera.getPosition());

    Engine::Renderer3D::submit(m_mesh, math::translate(math::mat4(1.f), math::vec3(2.f, 1.f, 4.f)));

    Engine::Renderer3D::endScene();

    Engine::Renderer2D::beginScene(m_orthoCamera);
    Engine::Renderer2D::renderSprite(Engine::Assets::get<Engine::Texture2D>("grass"), math::vec2(100, 100), math::vec2(100, 100), math::vec4(1, 1, 1, 1));
    Engine::Renderer2D::renderQuad(math::vec2(0, 0), math::vec2(100, 100), math::vec4(0, 1, 0, 1));
    Engine::Renderer2D::renderText("Hello, world!", m_font, math::vec2(0, 0), math::vec2(80, 80), math::vec4(1, 0, 0, 1));
    Engine::Renderer2D::endScene();

    Engine::Application::get().getWindow().setTitle(std::string("Sandbox FPS: " + std::to_string((int)floor(1000.f / timer.getMillis()))));
}

void Sandbox::onEvent(Engine::Event& event)
{
    m_perspectiveCamera.onEvent(event);
    m_orthoCamera.onEvent(event);
}