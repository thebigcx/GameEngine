#include "Sandbox.h"

Sandbox::Sandbox()
{
    Assets::add<Texture2D>("texture", Texture2D::create("Sandbox/assets/terrain.png"));
    Assets::add<Texture2D>("grass", Texture2D::create("Sandbox/assets/grass.png"));

    m_soundSource = SoundSource::loadFile("Sandbox/assets/monkeys.mp3");
    SoundEngine::play(*m_soundSource, true);
    
    m_framebuffer = Framebuffer::create(1280, 720);

    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));
    Renderer::setTarget(m_framebuffer);

    m_cubeMaterial = Material::create(Renderer3D::data.modelShader);
    m_cubeMaterial->setTexture(Assets::get<Texture2D>("grass"));
    m_cubeMaterial->shininess = 16.f;

    LightSetup lights;

    DirectionalLight dirLight;
    dirLight.direction = math::vec3(-0.2f, -1.f, -0.3f);
    dirLight.ambient = math::vec3(0.2f, 0.2f, 0.2f);
    dirLight.diffuse = math::vec3(0.4f, 0.4f, 0.4f);
    dirLight.specular = math::vec3(0.5f, 0.5f, 0.5f);

    lights.setDirectionalLight(dirLight);

    std::vector<PointLight> pointLights;
    for (int i = 0; i < 1; i++)
    {
        PointLight pointLight;
        pointLight.ambient = math::vec3(0.2f, 0.2f, 0.2f);
        pointLight.diffuse = math::vec3(0.8f, 0.8f, 0.8f);
        pointLight.specular = math::vec3(1.f, 1.f, 1.f);

        pointLight.constant = 1.f;
        pointLight.linear = 0.09f;
        pointLight.quadratic = 0.032f;
        
        pointLights.push_back(pointLight);
    }

    lights.setPointLights(pointLights);

    Renderer3D::setLights(lights);

    //m_model = Model::loadModel("Sandbox/assets/Donut.obj");
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

    Renderer::startFrame();

    Renderer3D::beginScene(m_perspectiveCamera);

    Renderer3D::submit(m_model, math::mat4(1.f));

    auto mesh = MeshFactory::cubeMesh(1.f);
    mesh->material = m_cubeMaterial;

    for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
    {
        Renderer3D::render(*mesh, math::scale(math::translate(math::mat4(1.f), math::vec3(i*2, j*2, 0)), math::vec3(2.f)));
    }

    Renderer3D::data.modelShader->setFloat3("pointLights[0].position", m_perspectiveCamera.getPosition());

    Renderer3D::render(*mesh, math::translate(math::mat4(1.f), math::vec3(2.f, 1.5f, 4.f)));

    m_perspectiveCamera.update();

    Renderer3D::endScene();

    Application::get().getWindow().setTitle(std::string("Sandbox FPS: " + std::to_string((int)floor(1000.f / timer.getMillis()))));

    Renderer::endFrame();
}

void Sandbox::handleEvent(const Event& event)
{
    if (event.type() == EventType::WindowResize)
    {
        m_framebuffer->resize(event.data().window.width, event.data().window.height);
    }
}