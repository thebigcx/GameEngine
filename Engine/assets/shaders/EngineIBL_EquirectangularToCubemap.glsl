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

layout (binding = 0) uniform sampler2D uEquirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);

vec2 sampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec2 uv = sampleSphericalMap(normalize(fsInput.localPos));
    color = vec4(texture(uEquirectangularMap, uv).rgb, 1.0);
}
