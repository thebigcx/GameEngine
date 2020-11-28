#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoord = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0); 
    gl_Position = pos.xyww;
}

#shader fragment

#version 460 core

in vec3 TexCoord;

out vec4 FragColor;

uniform samplerCube TextureCube;

void main()
{
    FragColor = texture(TextureCube, TexCoord);
}