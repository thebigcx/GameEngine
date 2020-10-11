#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;

out vec4 ocolor;

uniform mat4 projection;
uniform mat4 transform;

void main()
{
    ocolor = aColor;
    gl_Position = projection * transform * vec4(aPos.x, aPos.y, 0.0, 1.0);
}