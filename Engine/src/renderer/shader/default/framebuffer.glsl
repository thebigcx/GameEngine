#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out DATA
{
    vec2 texCoord;
} vs_out;

void main()
{
    vs_out.texCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
}

#shader fragment

#version 460 core

in DATA
{
    vec2 texCoord;
} fs_in;

out vec4 FragColor;

uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, fs_in.texCoord);
}