#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 Color;

uniform mat4 projection;
uniform mat4 transform;

void main()
{
    Color = aColor;
    gl_Position = projection * transform * vec4(aPos, 0.0, 1.0);
}