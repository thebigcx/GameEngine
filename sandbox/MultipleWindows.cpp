#include <FGL.h>

int main()
{
    Math::Random::initSeed();

    Window window(1280, 720, "Hello, Fast Game Library!");
    Window window1(1280, 720, "Window Mark 2");

    Shader shader;
    shader.create("shaders/default.vert", "shaders/default.frag");
    shader.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    shader.setUniform("projection", projection);

    glm::mat4 transform = glm::mat4(1.f);
    shader.setUniform("transform", transform);

    while (window.isOpen() || window1.isOpen())
    {
        if (window.isOpen())
            window.pollEvents();
        if (window1.isOpen())
        window1.pollEvents();

        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        Renderer::setContext(window);
        Renderer::clear(Color(0, 0, 0, 1));

        shader.bind();

        Renderer::display();


        Renderer::setContext(window1);
        Renderer::clear(Color(0, 0, 0, 1));

        Renderer::display();
    }

    return 0;
}