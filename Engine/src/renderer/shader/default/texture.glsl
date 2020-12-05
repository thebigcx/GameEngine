#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aTexIndex;

out DATA
{
    vec2 texCoord;
    vec4 color;
    float texIndex;
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
    vs_out.texIndex = aTexIndex;
    gl_Position = projection * transform * vec4(aPos, 1.0);
}

#shader fragment

#version 460 core

in DATA
{
    vec2 texCoord;
    vec4 color;
    float texIndex;
} fs_in;

out vec4 fragColor;

uniform sampler2D textures[32];

void main()
{
    vec4 color = texture(textures[int(fs_in.texIndex)], fs_in.texCoord) * fs_in.color;
    if (color.a < 0.1)
    {
        discard;
    }

    fragColor = color;
}
