#include <FGL/FGL.h>

int main()
{
    Window window(1280, 720, "Hello, Fast Game Library!");

    window.setIcon("res/icon.jpg");

    Shader shader("shaders/default.vert", "shaders/default.frag");
    shader.bind();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    shader.setUniform("projection", projection);

    glm::mat4 transform = glm::mat4(1.f);
    shader.setUniform("transform", transform);

    Batch batch;
    
    Quad quad(Vector2f(100, 100), Vector2f(100, 100), Color(1, 0, 0, 1));
    Quad quad1(Vector2f(300, 100), Vector2f(100, 100), Color(0, 1, 0, 1));
    Quad quad2(Vector2f(500, 100), Vector2f(100, 100), Color(0, 1, 1, 1));
    Quad quad3(Vector2f(700, 100), Vector2f(100, 100), Color(1, 1, 0, 1));
    batch.add(quad);
    batch.add(quad1);
    batch.add(quad2);
    batch.add(quad3);

    while (window.isOpen())
    {
        window.pollEvents();

        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        window.clear();

        quad.move(Vector2f(3, 0));

        batch.render();

        window.display();
    }

    return 0;
}