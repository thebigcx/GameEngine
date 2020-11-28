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

uniform mat4 projection;
uniform mat4 transform = mat4(1.f);

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

out vec4 FragColor;

uniform sampler2D Texture2D;

void main()
{
    vec4 color = texture(Texture2D, fs_in.texCoord) * fs_in.color;
    if (color.a < 0.1)
    {
        discard;
    }

    FragColor = color;
}
