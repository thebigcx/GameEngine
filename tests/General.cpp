#include <FGL/FGL.h>

int main()
{
    Math::Random::initSeed();

    Application application;
    Window window(1280, 720, "Hello, Fast Game Library!");
    Renderer::setContext(window);
    Application::get().setActiveWindow(window);

    Logger::init();

    window.setIcon("res/icon.jpg");

    Shader shader;
    shader.create("shaders/default.vert", "shaders/default.frag");
    shader.bind();

    Texture texture;
    texture.loadFile("res/terrain.png");
    texture.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    shader.setUniform("projection", projection);

    glm::mat4 transform = glm::mat4(1.f);
    shader.setUniform("transform", transform);

    Batch batch;
    
    Quad quad(Vector2f(100, 100), Vector2f(1000, 1000), Color(1, 1, 1, 1));
    batch.add(quad);

    quad.setTextureRect(FloatRect(15.f / 16.f, 0, 1.f / 16.f, 1.f / 16.f));
    quad.setOrigin(Vector2f(100, 100));

    while (window.isOpen())
    {
        window.pollEvents();

        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        Renderer::clear(Color(0, 0, 0, 1));

        shader.bind();

        quad.rotate(1);

        batch.update();
        Renderer::renderBatch(batch);

        Renderer::display();
    }

    return 0;
}