#include <FGL/FGL.h>

int main()
{
    Application application;
    auto& window = Application::get().getWindow();

    SoundBuffer buffer;
    buffer.load(SoundBuffer::FileType::MP3, "res/beat.mp3");
    Source source; source.create();

    window.setIcon("res/icon.jpg");

    {
        Shader textureShader;
        textureShader.create("shaders/texture.vert", "shaders/texture.frag");

        ShaderLibrary::add("texture", textureShader);
        ShaderLibrary::get("texture").bind();
    }

    Texture texture;
    texture.loadFile("res/terrain.png");
    texture.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    ShaderLibrary::get("texture").setUniform("projection", projection);

    Batch batch(ShaderLibrary::get("texture"));
    
    Quad quad(Vector2f(100, 100), Vector2f(100, 100), Color(1, 1, 1, 1));
    batch.add(quad);

    quad.setTextureRect(FloatRect(15.f / 16.f, 0, 1.f / 16.f, 1.f / 16.f));
    quad.setOrigin(quad.getSize() / 2.f);

    while (window.isOpen())
    {
        window.pollEvents();

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }

        //SoundManager::playFromSource(buffer, source);
        SoundManager::play2D(buffer);

        if (Keyboard::isKeyPressed(Keyboard::Key::A))
        {
            quad.move(Vector2f(-10, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::D))
        {
            quad.move(Vector2f(10, 0));
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::W))
        {
            quad.move(Vector2f(0, 10));
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::S))
        {
            quad.move(Vector2f(0, -10));
        }

        Renderer::startRender();

        batch.update();
        Renderer::renderBatch(batch);

        Renderer::finishRender();
    }

    source.destroy();
    buffer.destroy();
    SoundManager::cleanUp();

    return 0;
}

