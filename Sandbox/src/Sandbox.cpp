#include "Sandbox.h"

Sandbox::Sandbox()
{
    Shared<Texture2D> texture = Texture2D::create("Sandbox/assets/terrain.png");
    Assets::add<Texture2D>("texture", texture);

    m_soundSource = SoundSource::loadFile("Sandbox/assets/monkeys.mp3");
    SoundEngine::play(*m_soundSource, true);
    
    m_framebuffer = Framebuffer::create(1280, 720);

    m_font = TrueTypeFont::create("Sandbox/assets/minecraftia.ttf", 48);

    Renderer2D::setClearColor(math::vec4(0, 0, 0, 1));

    m_animation = Animation::create(Assets::get<Texture2D>("texture"));
    m_animation->setFrames({ 
        math::frect(32, 240, 16, 16), 
        math::frect(48, 240, 16, 16),
        math::frect(64, 240, 16, 16),
        math::frect(80, 240, 16, 16)
    });
    m_animation->setFrameInterval(100.f);
}

void Sandbox::update()
{
    Timer timer;
    auto dt = Time::getDelta();
    const float speed = 0.5;

    if (Input::isKeyPressed(Key::A))
        m_camera.translate(math::vec2(-speed * dt, 0));

    if (Input::isKeyPressed(Key::D))
        m_camera.translate(math::vec2(speed * dt, 0));

    if (Input::isKeyPressed(Key::W))
        m_camera.translate(math::vec2(0, speed * dt));

    if (Input::isKeyPressed(Key::S))
        m_camera.translate(math::vec2(0, -speed * dt));

    if (Input::isKeyPressed(Key::Escape))
    {
        Application::get().quit();
    }

    rot++;

    m_framebuffer->bind();
    Renderer2D::clear();

    m_animation->update();

    Renderer2D::data.transform = m_camera.getViewMatrix();
    Renderer2D::startBatch();

    for (int x = 0; x < 1000; x++)
    for (int y = 0; y < 1000; y++)
    {
        Renderer2D::renderQuad(math::vec2(x * 100, y * 100), math::vec2(100, 100), math::vec4(1, 1, 1, 1));
    }

    Renderer2D::renderSprite(Assets::get<Texture2D>("texture"), math::vec2(100, 100), math::vec2(100, 100), m_animation->getCurrentFrame());
    
    
    
    Renderer2D::endBatch();

    Renderer2D::renderText("Hello, world!", m_font, math::vec2(500, 500), math::vec2(80, 80), math::vec4(1, 0, 0, 1));

    //auto mesh = MeshFactory::cubeMesh(100);
    //Renderer3D::render(*mesh, math::mat4(1.f), Assets::get<Texture2D>("texture"), Renderer2D::data.textureShader);
    //Renderer3D::render(*mesh, math::mat4(1.f), Renderer2D::data.whiteTexture, Renderer2D::data.textureShader);

    m_framebuffer->unbind();

    Renderer2D::clear();

    Renderer2D::renderFramebuffer(*m_framebuffer);

    Application::get().getWindow().setTitle(std::string("Sandbox FPS: " + std::to_string((int)floor(1000.f / timer.getMillis()))));
}

void Sandbox::handleEvent(const Event& event)
{
    if (event.type() == EventType::WindowResize)
    {
        m_framebuffer->resize(event.data().window.width, event.data().window.height);
    }
}