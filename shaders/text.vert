#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;
out vec4 color;

uniform mat4 projection;
uniform mat4 transform = mat4(1.f);

void main()
{
    texCoord = aTexCoord;
    gl_Position = projection * transform * vec4(aPos, 0.0, 1.0);
}