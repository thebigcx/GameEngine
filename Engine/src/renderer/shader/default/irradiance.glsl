#shader vertex

layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = aPos;
    gl_Position = projection * view * vec4(localPos, 1.0);
}

#shader fragment

layout (location = 0) out vec4 fragColor;

in vec3 localPos;

layout (binding = 0) uniform samplerCube environmentMap;

const float PI = 3.141592653589793442384626433832;

void main()
{
    vec3 normal = normalize(localPos);

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

        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; // N? presume normal, could be wrong

        irradiance += texture(environmentMap, sampleVec).rgb * cos(theta) * sin(theta);
        samples++;
    }

    irradiance = PI * irradiance * (1.0 / float(samples));

    fragColor = vec4(irradiance, 1.0);
}