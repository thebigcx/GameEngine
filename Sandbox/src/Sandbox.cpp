#include "Sandbox.h"

Sandbox::Sandbox()
{
    Assets::add<Texture2D>("texture", Texture2D::create("Sandbox/assets/terrain.png"));
    Assets::add<Texture2D>("grass", Texture2D::create("Sandbox/assets/grass.png"));

    m_soundSource = SoundSource::loadFile("Sandbox/assets/monkeys.mp3");
    SoundEngine::play(*m_soundSource, true);

    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));

    m_cubeMaterial = Material::create(Renderer3D::data.modelShader);
    m_cubeMaterial->setTexture(Assets::get<Texture2D>("grass"));
    m_cubeMaterial->shininess = 16.f;

    LightSetup lights;

    lights.setSkylight(0.01f);

    DirectionalLight dirLight;
    dirLight.direction = math::vec3(-0.2f, -1.f, -0.3f);
    dirLight.intensity = 0.02f;
    dirLight.color = math::vec3(1, 1, 1);
    dirLight.specular = 1.f;

    lights.setDirectionalLight(dirLight);

    std::vector<PointLight> pointLights;
    for (int i = 0; i < 1; i++)
    {
        PointLight pointLight;

        pointLight.color = math::vec3(1, 1, 1);
        pointLight.intensity = 1.f;
        pointLight.specular = 1.f;
        pointLight.attenuation = 0.08f;
        
        pointLights.push_back(pointLight);
    }

    lights.setPointLights(pointLights);

    std::vector<SpotLight> spotLights;
    for (int i = 0; i < 1; i++)
    {
        SpotLight spotLight;

        spotLight.color = math::vec3(1, 1, 1);
        spotLight.intensity = 1.f;
        spotLight.specular = 1.f;
        spotLight.attenuation = 0.08f;
        spotLight.cutoff = cos(math::asRadians(12.5f));
        spotLight.outerCutoff = cos(math::asRadians(17.5f));
        spotLight.direction = math::vec3(0, 0, 1);
        
        spotLights.push_back(spotLight);
    }

    lights.setSpotLights(spotLights);

    Renderer3D::setLights(lights);

    m_model = Model::loadModel("Sandbox/assets/model/backpack.obj");
    
    Application::get().setCursorEnabled(false);
}

void Sandbox::update()
{
    Timer timer;

    if (Input::isKeyPressed(Key::Escape))
    {
        Application::get().quit();
    }

    //Renderer::startFrame();

    Renderer3D::beginScene(m_perspectiveCamera);

    Renderer3D::submit(m_model, math::mat4(1.f));

    auto mesh = MeshFactory::cubeMesh(1.f);
    mesh->material = m_cubeMaterial;

    for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
    {
        Renderer3D::submit(mesh, math::scale(math::translate(math::mat4(1.f), math::vec3(i * 2, j * 2, 0)), math::vec3(2.f)));
    }

    Renderer3D::data.modelShader->setFloat3("spotLights[0].position", m_perspectiveCamera.getPosition());
    Renderer3D::data.modelShader->setFloat3("spotLights[0].direction", m_perspectiveCamera.getDirection());

    Renderer3D::submit(mesh, math::translate(math::mat4(1.f), math::vec3(2.f, 1.5f, 4.f)));

    m_perspectiveCamera.update();

    Renderer3D::endScene();

    Application::get().getWindow().setTitle(std::string("Sandbox FPS: " + std::to_string((int)floor(1000.f / timer.getMillis()))));

    //Renderer::endFrame();
}

void Sandbox::handleEvent(const Event& event)
{
    if (event.type() == EventType::WindowResize)
    {
        Renderer3D::data.hdrBuffer->resize(event.data().window.width, event.data().window.height);
    }
}