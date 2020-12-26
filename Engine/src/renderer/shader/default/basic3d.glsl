#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out DATA
{
    vec2 texCoord;
    vec3 normal;
} vs_out;

layout (std140, binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 transform = mat4(1.f);

void main()
{
    vs_out.normal = aNormal;
    vs_out.texCoord = aTexCoord;

    gl_Position = projection * view * transform * vec4(aPos, 1.0);
}

#shader fragment

#define MAX_LIGHTS 64

struct Material
{
    sampler2D albedo;
    sampler2D normal;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D depth;

    float metallicScalar;
    float roughnessScalar;

    bool usingAlbedo;
    bool usingNormal;
    bool usingMetallic;
    bool usingRoughness;
    bool usingAo; // Could combine them all in a single int
    bool usingDepth;

    vec3 albedoColor;
};

struct PointLight
{
    vec3 position;
    vec3 radiance;
    float intensity;
    float attenuation;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 radiance;
    float intensity;
};

in DATA
{
    vec2 texCoord;
    vec3 normal;
} fs_in;

out vec4 fragColor;

uniform vec3 cameraPos;

uniform Material material;

uniform PointLight pointLights[MAX_LIGHTS];
uniform DirectionalLight directionalLight;
uniform int usingDirectionalLight;
uniform int numPointLights;

uniform float skyLight;
uniform float exposure;

void main()
{
    fragColor = texture(material.albedo, fs_in.texCoord);
}