#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

uniform mat4 lightSpaceMatrix;
uniform mat4 transform = mat4(1.f);

void main()
{
    gl_Position = lightSpaceMatrix * transform * vec4(aPos, 1.0);
}

#shader fragment

void main()
{

}