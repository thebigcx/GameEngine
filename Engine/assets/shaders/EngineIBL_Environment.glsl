#shader vertex

layout (location = 0) in vec3 aPos;

out Params
{
    vec3 localPos;
} vsOutput;

layout (std140, binding = 0) uniform matrices
{
    mat4 uProjection;
    mat4 uView;
};

void main()
{
    vsOutput.localPos = aPos;

    mat4 rotView = mat4(mat3(uView));
    vec4 clipPos = uProjection * rotView * vec4(aPos, 1.0);

    gl_Position = clipPos.xyww;
}

#shader fragment

in Params
{
    vec3 localPos;
} fsInput;

layout (location = 0) out vec4 color;

layout (binding = 0) uniform samplerCube uEnvironment;

void main()
{
    vec3 envColor = texture(uEnvironment, fsInput.localPos).rgb;
    color = vec4(envColor, 1.0);
}
