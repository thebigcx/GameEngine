#shader vertex

layout (location = 0) in vec3 aPos;

out Params
{
    vec3 localPos;
} vsOutput;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    vsOutput.localPos = aPos;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}

#shader fragment

layout (location = 0) out vec4 color;

in Params
{
    vec3 localPos;
} fsInput;

layout (binding = 0) uniform samplerCube uEnvironmentMap;

const float PI = 3.14159;

void main()
{
    vec3 normal = normalize(fsInput.localPos);

    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(up, normal);
    up = cross(normal, right);

    float sampleDelta = 0.025;
    float samples = 0.0;

    for (float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    for (float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
    {
        vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal;

        irradiance += texture(uEnvironmentMap, sampleVec).rgb * cos(theta) * sin(theta);
        samples++;
    }

    irradiance = PI * irradiance * (1.0 / samples);

    color = vec4(irradiance, 1.0);
}