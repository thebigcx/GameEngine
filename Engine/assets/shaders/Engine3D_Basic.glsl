#shader vertex

// Vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

// Vertex shader output
out Params
{
    vec2 texCoord;
} vsOutput;

layout (std140, binding = 0) uniform matrices
{
    mat4 uProjection;
    mat4 uView;
};

uniform mat4 uLightSpaceMatrix;
uniform mat4 uTransform = mat4(1.f);

void main()
{
    vsOutput.texCoord = aTexCoord;
    gl_Position = uProjection * uView * vec4(aPos, 1.0);
}

#shader fragment

#define MAX_LIGHTS 64

// Structure representing a PBR material
struct Material
{
    float metallic;
    float roughness;

    float albedoMapToggle;
    float normalMapToggle;
    float metallicMapToggle;
    float roughnessMapToggle;
    float lightmapToggle;
    float emissionMapToggle;

    vec3 albedoColor;
};

// Structures representing types of lights
struct PointLight
{
    vec3 position;
    vec3 radiance;
    float intensity;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 radiance;
    float intensity;
};

struct SkyLight
{
    vec3 radiance;
    float intensity;
};

in Params
{
    vec2 texCoord;
} fsInput;

// Material texture samplers
layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uNormalMap;
layout (binding = 2) uniform sampler2D uMetallicMap;
layout (binding = 3) uniform sampler2D uRoughnessMap;
layout (binding = 4) uniform sampler2D uLightmap;
layout (binding = 5) uniform sampler2D uHeightMap;
layout (binding = 6) uniform sampler2D uEmissionMap;

// Environment texture samplers
layout (binding = 7) uniform samplerCube uIrradianceMap;
layout (binding = 8) uniform samplerCube uPrefilterMap;
layout (binding = 9) uniform sampler2D uBrdfLUT;

// Shadow map sampler
layout (binding = 10) uniform sampler2D uShadowMap;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

// Uniforms
uniform vec3 uCameraPos;
uniform Material uMaterial;

uniform PointLight uPointLights[MAX_LIGHTS];
uniform int uNumPointLights;

uniform DirectionalLight uDirectionalLight;
uniform int uUsingDirectionalLight;

uniform SkyLight uSkyLight;

void main()
{
    color = vec4(texture(uAlbedoMap, fsInput.texCoord).rgb, 1.0);
}