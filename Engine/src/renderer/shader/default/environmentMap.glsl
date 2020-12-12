#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out DATA
{
    vec3 normal;
    vec3 position;
    vec3 cameraPos;
} vs_out;

layout (std140, binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
    //vec3 cameraPos;
};

uniform vec3 cameraPos;
uniform mat4 transform = mat4(1.f);

void main()
{
    vs_out.normal = mat3(transpose(inverse(transform))) * aNormal;
    vs_out.position = vec3(transform * vec4(aPos, 1.0));
    vs_out.cameraPos = cameraPos;
    gl_Position = projection * view * vec4(vs_out.position, 1.0);
}

#shader fragment

in DATA
{
    vec3 normal;
    vec3 position;
    vec3 cameraPos;
} fs_in;

out vec4 FragColor;

struct Material
{
    float shininess;
};

uniform Material material;

uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(fs_in.position - fs_in.cameraPos);
    vec3 R = reflect(I, normalize(fs_in.normal));
    //vec3 R = refract(I, normalize(fs_in.normal), 1.00 / 1.309);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
