#version 460 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texture_;
uniform vec4 textColor;

void main()
{
    vec4 sampled = vec4(1, 1, 1, texture(texture_, texCoord).r);
    if (sampled.a < 0.1)
    {
        discard;
    }
    fragColor = sampled * vec4(textColor.rgb, 1);
}