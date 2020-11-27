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

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Texture2D;
uniform float exposure = 1;

void main()
{   
    const float gamma = 2.2;
    vec3 hdrColor = texture(Texture2D, TexCoord).rgb;

    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(mapped, 1.0);
}