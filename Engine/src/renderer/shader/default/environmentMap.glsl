#shader vertex

layout (location = 0) in vec3 aPos;

out vec3 localPos;

layout (std140, binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
};

void main()
{
    localPos = aPos;

    mat4 rotView = mat4(mat3(view));
    vec4 clipPos = projection * rotView * vec4(localPos, 1.0);

    gl_Position = clipPos.xyww;
}

#shader fragment

in vec3 localPos;

out vec4 FragColor;

layout (binding = 0) uniform samplerCube environmentMap;

void main()
{
    //vec3 envColor = texture(environmentMap, localPos).rgb;
    vec3 envColor = textureLod(environmentMap, localPos, 1.2).rgb;
    FragColor = vec4(envColor, 1.0);
}
