#include <FGL/FGL.h>

int main()
{
    Math::Random::initSeed();

    Application application;
    auto& window = Application::get().getWindow();

    SoundManager::init();

    SoundBuffer buffer = SoundBuffer::loadWAV("res/beat_2.wav");
    Source source; source.create();

    window.setIcon("res/icon.jpg");

    Shader textureShader;
    textureShader.create("shaders/texture.vert", "shaders/texture.frag");

    ShaderLibrary::add("texture", textureShader);
    ShaderLibrary::get("texture").bind();

    Texture texture;
    texture.loadFile("res/terrain.png");
    texture.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    textureShader.setUniform("projection", projection);

    Batch batch(ShaderLibrary::get("texture"));
    
    Quad quad(Vector2f(100, 100), Vector2f(100, 100), Color(1, 1, 1, 1));
    batch.add(quad);
    std::vector<Quad> quads;

    quad.setTextureRect(FloatRect(15.f / 16.f, 0, 1.f / 16.f, 1.f / 16.f));
    quad.setOrigin(quad.getSize() / 2.f);    

    while (window.isOpen())
    {
        window.pollEvents();

        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        SoundManager::playFromSource(buffer, source);

        Renderer::clear(Color(0, 0, 0, 1));

        

        batch.update();
        Renderer::renderBatch(batch);

        Renderer::display();
    }

    source.destroy();
    buffer.destroy();
    SoundManager::cleanUp();

    return 0;
}
