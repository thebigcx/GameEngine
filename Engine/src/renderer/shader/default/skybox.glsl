#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;

out DATA
{
    vec3 texCoord;
} vs_out;

layout (std140, binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    vs_out.texCoord = aPos;
    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0); 
    gl_Position = pos.xyww;
}

#shader fragment

#version 460 core

in DATA
{
    vec3 texCoord;
} fs_in;

out vec4 FragColor;

uniform samplerCube TextureCube;

void main()
{
    FragColor = texture(TextureCube, fs_in.texCoord);
}