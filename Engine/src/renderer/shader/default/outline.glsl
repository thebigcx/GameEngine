#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

layout (binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 transform = mat4(1.f);

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
}

#shader fragment

layout (location = 0) out vec4 fragColor;

uniform vec3 outlineColor;

void main()
{
    fragColor = vec4(outlineColor, 1.0);   
}