#include "../src/Window.h"

#include "../src/Batch.h"
#include "../src/Quad.h"
#include "../src/Shader.h"
#include "../src/util/maths/MathGL.h"

int main()
{
    Window window(1280, 720, "Hello, Fast Game Library!");

    Shader shader("shaders/default.vert", "shaders/default.frag");
    shader.bind();
    glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
    shader.setUniform("projection", projection);
    glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(100.f, 100.f, 1.f));
    shader.setUniform("model", model);

    Batch batch;
    
    Quad quad(Vector2f(100, 100));
    batch.add(quad);

    while (window.isOpen())
    {
        window.pollEvents();

        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }

        window.clear();

        batch.render();

        window.display();
    }

    return 0;
}