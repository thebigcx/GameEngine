#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out DATA
{
    vec3 normal;
    vec3 position;
} vs_out;

layout (std140) uniform matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 transform = mat4(1.f);

void main()
{
    vs_out.normal = mat3(transpose(inverse(transform))) * aNormal;
    vs_out.position = vec3(transform * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(vs_out.position, 1.0);
}

#shader fragment

#version 460 core

in DATA
{
    vec3 normal;
    vec3 position;
} fs_in;

out vec4 FragColor;

struct Material
{
    float shininess;
};

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform Material material;

void main()
{
    vec3 I = normalize(fs_in.position - cameraPos);
    vec3 R = reflect(I, normalize(fs_in.normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
