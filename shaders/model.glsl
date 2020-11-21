#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 transform = mat4(1.f);

void main()
{
    Normal = aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * transform * vec4(aPos, 1.0);
}

#shader fragment

#version 460 core

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D Texture2D;

void main()
{
    vec4 color = texture(Texture2D, TexCoord);
    FragColor = color;
}
