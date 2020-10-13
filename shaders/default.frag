#version 460 core

in vec2 TexCoord;
in vec4 Color;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    vec4 color = texture(Texture, TexCoord) * Color;
    if (Color.a < 0.1)
    {
        discard;
    }

    FragColor = color;
}