#shader vertex

#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0, 1);
    TexCoord = aTexCoord;
}

#shader fragment

#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture2D;

void main()
{
    FragColor = texture(Texture2D, TexCoord);
}