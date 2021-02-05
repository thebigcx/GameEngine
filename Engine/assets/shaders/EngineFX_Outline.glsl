#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

layout (binding = 0) uniform matrices
{
    mat4 uProjection;
    mat4 uView;
};

uniform mat4 uTransform = mat4(1.f);

void main()
{
    gl_Position = uProjection * uView * uTransform * vec4(aPos, 1.0);
}

#shader fragment

layout (location = 0) out vec4 color;

uniform vec3 uOutlineColor;

void main()
{
    color = vec4(uOutlineColor, 1.0);   
}