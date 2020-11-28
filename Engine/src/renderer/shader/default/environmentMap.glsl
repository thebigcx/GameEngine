#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec3 Position;

uniform mat4 projection;
uniform mat4 view = mat4(1.f);
uniform mat4 transform = mat4(1.f);

void main()
{
    Normal = mat3(transpose(inverse(transform))) * aNormal;
    Position = vec3(transform * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(Position, 1.0);
}

#shader fragment

#version 460 core

in vec3 Normal;
in vec3 Position;

out vec4 FragColor;

struct Material
{
    float shininess;
};

uniform vec3 viewPos;
uniform samplerCube skybox;
uniform Material material;

void main()
{
    vec3 I = normalize(Position - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
