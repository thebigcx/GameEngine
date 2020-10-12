#version 460 core

in vec4 Color;

out vec4 FragColor;

void main()
{
    if (Color.a < 0.1)
    {
        discard;
    }
    FragColor = Color;
}