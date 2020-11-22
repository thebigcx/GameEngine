#include "Sandbox.h"

Sandbox::Sandbox()
{
    Assets::add<Texture2D>("texture", Texture2D::create("Sandbox/assets/terrain.png"));
    Assets::add<Texture2D>("grass", Texture2D::create("Sandbox/assets/grass.png"));

    

    m_soundSource = SoundSource::loadFile("Sandbox/assets/monkeys.mp3");
    SoundEngine::play(*m_soundSource, true);
    
    m_framebuffer = Framebuffer::create(1280, 720);

    m_font = TrueTypeFont::create("Sandbox/assets/minecraftia.ttf", 48);

    RenderCommand::setClearColor(math::vec4(0, 0, 0, 1));
    Renderer::setTarget(m_framebuffer);

    m_animation = Animation::create(Assets::get<Texture2D>("texture"));
    m_animation->setFrames({ 
        math::frect(32, 240, 16, 16), 
        math::frect(48, 240, 16, 16),
        math::frect(64, 240, 16, 16),
        math::frect(80, 240, 16, 16)
    });
    m_animation->setFrameInterval(100.f);

    m_particleSystem = ParticleSystem::create(Assets::get<Texture2D>("texture"));
    for (int i = 0 ; i < 10 ; i++)
    {
        Particle p;
        p.life = 1.f;
        p.velocity = math::vec2(math::random::generate(-5, 5), math::random::generate(-5, 5));
        m_particleSystem->particles.push_back(p);
    }

    m_cubeMaterial = Material::create(Renderer3D::data.modelShader);
    m_cubeMaterial->setTexture(Assets::get<Texture2D>("grass"));

    Renderer3D::data.modelShader->bind();

    Renderer3D::data.modelShader->setFloat3("dirLight.direction", math::vec3(-0.2f, -1.f, -0.3f));
    Renderer3D::data.modelShader->setFloat3("dirLight.ambient", math::vec3(0.05f, 0.05f, 0.05f));
    Renderer3D::data.modelShader->setFloat3("dirLight.diffuse", math::vec3(0.4f, 0.4f, 0.4f));
    Renderer3D::data.modelShader->setFloat3("dirLight.specular", math::vec3(0.5f, 0.5f, 0.5f));

    Renderer3D::data.modelShader->setInt("numPointLights", 2);
    Renderer3D::data.modelShader->setFloat3("pointLights[0].position", math::vec3(5.f, 5.f, 3.f));
    Renderer3D::data.modelShader->setFloat3("pointLights[0].ambient", math::vec3(0.05f, 0.05f, 0.05f));
    Renderer3D::data.modelShader->setFloat3("pointLights[0].diffuse", math::vec3(0.8f, 0.8f, 0.8f));
    Renderer3D::data.modelShader->setFloat3("pointLights[0].specular", math::vec3(1.0f, 1.0f, 1.0f));
    Renderer3D::data.modelShader->setFloat("pointLights[0].constant", 1.0f);
    Renderer3D::data.modelShader->setFloat("pointLights[0].linear", 0.09);
    Renderer3D::data.modelShader->setFloat("pointLights[0].quadratic", 0.032);

    Renderer3D::data.modelShader->setFloat3("pointLights[1].position", math::vec3(2.f, 3.4f, 5.f));
    Renderer3D::data.modelShader->setFloat3("pointLights[1].ambient", math::vec3(0.05f, 0.05f, 0.05f));
    Renderer3D::data.modelShader->setFloat3("pointLights[1].diffuse", math::vec3(0.8f, 0.8f, 0.8f));
    Renderer3D::data.modelShader->setFloat3("pointLights[1].specular", math::vec3(1.0f, 1.0f, 1.0f));
    Renderer3D::data.modelShader->setFloat("pointLights[1].constant", 1.0f);
    Renderer3D::data.modelShader->setFloat("pointLights[1].linear", 0.09);
    Renderer3D::data.modelShader->setFloat("pointLights[1].quadratic", 0.032);

    //m_model = Model::loadModel("Sandbox/assets/Rock1.obj");
    m_model = Model::loadModel("Sandbox/assets/Donut.obj");
    //m_model = Model::loadModel("Sandbox/assets/model/Rock1.obj");
    
    Application::get().setCursorEnabled(false);
}

void Sandbox::update()
{
    //if (Time::getTime() < 100000)
    {
        
    }
    Timer timer;
    auto dt = Time::getDelta();
    const float speed = 0.5;

    if (Input::isKeyPressed(Key::A))
        m_camera.translate(math::vec2(speed * dt, 0));

    if (Input::isKeyPressed(Key::D))
        m_camera.translate(math::vec2(-speed * dt, 0));

    if (Input::isKeyPressed(Key::W))
        m_camera.translate(math::vec2(0, speed * dt));

    if (Input::isKeyPressed(Key::S))
        m_camera.translate(math::vec2(0, -speed * dt));

    if (Input::isKeyPressed(Key::Escape))
    {
        Application::get().quit();
    }

    rot++;

    Renderer::startFrame();

    /*m_animation->update();

    m_particleSystem->update();
    m_particleSystem->render();

    Renderer2D::startBatch(m_camera.getViewMatrix());

    Renderer2D::renderSprite(Assets::get<Texture2D>("texture"), math::vec2(100, 100), math::vec2(100, 100), m_animation->getCurrentFrame());

    Renderer2D::endBatch();

    Renderer2D::renderText("Hello, world!", m_font, math::vec2(500, 500), math::vec2(80, 80), math::vec4(1, 0, 0, 1));*/

    math::mat4 transform(1.f);
    transform = math::scale(transform, math::vec3(1.f));
    Renderer3D::data.modelShader->bind();
    Renderer3D::data.modelShader->setMatrix4("view", m_perspectiveCamera.getViewMatrix());
    Renderer3D::data.modelShader->setFloat3("viewPos", m_perspectiveCamera.getPosition());

    Renderer3D::data.modelShader->setFloat("material.shininess", 16.f);

    Renderer3D::submit(m_model, transform);

    auto mesh = MeshFactory::cubeMesh(1.f);

    for (int i = 0; i < 10; i++)
    for (int j = 0; j < 10; j++)
    {
        //Renderer3D::render(*mesh, math::scale(math::translate(math::mat4(1.f), math::vec3(i*2, j*2, 0)), math::vec3(2.f)), m_cubeMaterial);
    }

    Renderer3D::render(*mesh, math::translate(math::mat4(1.f), math::vec3(2.f, 1.5f, 4.f)), m_cubeMaterial);

    m_perspectiveCamera.update();

    Renderer::endFrame();

    Application::get().getWindow().setTitle(std::string("Sandbox FPS: " + std::to_string((int)floor(1000.f / timer.getMillis()))));
}

void Sandbox::handleEvent(const Event& event)
{
    if (event.type() == EventType::WindowResize)
    {
        m_framebuffer->resize(event.data().window.width, event.data().window.height);
    }
}