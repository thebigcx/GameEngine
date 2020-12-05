#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;

out DATA
{
    vec2 texCoord;
    vec4 color;
} vs_out;

layout (std140, binding = 2) uniform matrices
{
    mat4 projection;
    mat4 transform;
};

void main()
{
    vs_out.texCoord = aTexCoord;
    vs_out.color = aColor;
    gl_Position = projection * transform * vec4(aPos, 1.0);
}

#shader fragment

#version 460 core

in DATA
{
    vec2 texCoord;
    vec4 color;
} fs_in;

out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    vec4 color = texture(textureSampler, fs_in.texCoord) * fs_in.color;
    if (color.a < 0.1)
    {
        discard;
    }

    fragColor = color;
}
