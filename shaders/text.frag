#version 460 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture;

void main()
{
    vec4 sampled = vec4(1, 1, 1, texture(Texture, TexCoord).r);
    if (sampled.a < 0.1)
    {
        discard;
    }
    //FragColor = sampled;
    FragColor = vec4(1, 0, 0, 1);
}