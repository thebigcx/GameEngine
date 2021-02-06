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
    vec3 normal;
    vec3 worldPos; // The frag's position in world space
    vec4 worldPosLightSpace;
    mat3 worldNormals; // The Tangent, Binormal, Normal matrix
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
    vec3 aBitangent = cross(aNormal, aTangent);

    vsOutput.normal = transpose(inverse(mat3(uTransform))) * aNormal;
    vsOutput.texCoord = aTexCoord;
    vsOutput.worldNormals = mat3(uTransform) * mat3(aTangent, aBitangent, aNormal);
    vsOutput.worldPos = vec3(uTransform * vec4(aPos, 1.0));
    vsOutput.worldPosLightSpace = uLightSpaceMatrix * vec4(vsOutput.worldPos, 1.0);

    gl_Position = uProjection * uView * vec4(vsOutput.worldPos, 1.0);
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
    vec3 normal;
    vec3 worldPos;
    vec4 worldPosLightSpace;
    mat3 worldNormals;
} fsInput;

struct InternalParams
{
    vec3 albedo;
    vec3 normal;
    vec3 emission;

    float roughness;
    float metallic;
    float ao;

    float NdotV;
    vec3 view;
} m_params;

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

const float PI = 3.1415926535897932384626433832795028841971693993751058209749445923;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    mat3 TBN;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Offset the uv coordinates based on the material's heightmap
vec2 parallaxMapping(vec2 texCoord, vec3 viewDir)
{
    const float heightScale = 0.1;

    const float minLayers = 8;
    const float maxLayers = 32;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoord = P / numLayers;

    vec2 currentTexCoord = texCoord;
    float currentDepthMapValue = 1.0 - texture(uHeightMap, currentTexCoord).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoord -= deltaTexCoord;
        currentDepthMapValue = 1.0 - texture(uHeightMap, currentTexCoord).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoord = currentTexCoord + deltaTexCoord;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1.0 - texture(uHeightMap, prevTexCoord).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);

    return finalTexCoord;
}

// Calculate whether the fragment is in shadow
float shadowCalculation(vec4 worldPosLightSpace)
{
    vec3 projCoords = worldPosLightSpace.xyz / worldPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(uShadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 lighting(vec3 F0)
{
    vec3 Lo = vec3(0.0);

    // ----------------------------Point Lights-------------------------------------

    for (int i = 0; i < uNumPointLights; i++)
    {
        vec3 lightPos = uPointLights[i].position;

        vec3 lightDir = normalize(lightPos - fsInput.worldPos);
        vec3 H = normalize(m_params.view + lightDir);

        float distance = length(lightPos - fsInput.worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = uPointLights[i].radiance * attenuation;

        float NDF = distributionGGX(m_params.normal, H, m_params.roughness);
        float G = geometrySmith(m_params.normal, m_params.view, lightDir, m_params.roughness);
        vec3 F = fresnelSchlick(max(dot(H, m_params.view), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - m_params.metallic;

        float NdotL = max(dot(m_params.normal, lightDir), 0.0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * m_params.NdotV * NdotL;
        vec3 specular = numerator / max(denominator, 0.001);

        vec3 pointLightContribution = (kD * m_params.albedo / PI + specular) * radiance * NdotL;
        pointLightContribution *= uPointLights[i].intensity;

        Lo += pointLightContribution;
    }
    
    // ----------------------------Directional Light-------------------------------------
    if (uUsingDirectionalLight == 1)
    {
        vec3 lightDir = normalize(-uDirectionalLight.direction);
        vec3 H = normalize(m_params.view + lightDir);

        vec3 radiance = uDirectionalLight.radiance;

        float NDF = distributionGGX(m_params.normal, H, m_params.roughness);
        float G = geometrySmith(m_params.normal, m_params.view, lightDir, m_params.roughness);
        vec3 F = fresnelSchlick(max(dot(H, m_params.view), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - m_params.metallic;

        float NdotL = max(dot(m_params.normal, lightDir), 0.0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * m_params.NdotV * NdotL;
        vec3 specular = numerator / max(denominator, 0.001);

        vec3 dirLightContribution = (kD * m_params.albedo / PI + specular) * radiance * NdotL;
        dirLightContribution *= uDirectionalLight.intensity;

        Lo += dirLightContribution;
    }

    return Lo;
}

vec3 IBL(vec3 F0)
{
    vec3 F = fresnelSchlickRoughness(m_params.NdotV, F0, m_params.roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - m_params.metallic;
    
    vec3 irradiance = texture(uIrradianceMap, m_params.normal).rgb;
    vec3 diffuse = irradiance * m_params.albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(uPrefilterMap, reflect(-m_params.view, m_params.normal), m_params.roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(uBrdfLUT, vec2(max(dot(m_params.normal, m_params.view), 0.0), m_params.roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    return kD * diffuse + specular;
}

void main()
{
    // Retrieve all material attributes
    m_params.albedo = uMaterial.albedoMapToggle > 0.5 ? texture(uAlbedoMap, fsInput.texCoord).rgb : uMaterial.albedoColor;
    m_params.metallic = uMaterial.metallicMapToggle > 0.5 ? texture(uMetallicMap, fsInput.texCoord).r : uMaterial.metallic;
    m_params.roughness = uMaterial.roughnessMapToggle > 0.5 ? texture(uRoughnessMap, fsInput.texCoord).r : uMaterial.roughness;
    m_params.ao = uMaterial.lightmapToggle > 0.5 ? texture(uLightmap, fsInput.texCoord).r : 1.0;
    m_params.emission = uMaterial.emissionMapToggle > 0.5 ? texture(uEmissionMap, fsInput.texCoord).rgb : vec3(0.0);

    // Get normal, from map if necessary
    m_params.normal = normalize(fsInput.normal);
    if (uMaterial.normalMapToggle > 0.5)
    {
        m_params.normal = normalize(2.0 * texture(uNormalMap, fsInput.texCoord).rgb - 1.0);
        m_params.normal = normalize(fsInput.worldNormals * m_params.normal);
    }

    m_params.view = normalize(uCameraPos - fsInput.worldPos);
    m_params.NdotV = max(dot(m_params.normal, m_params.view), 0.0);

    const float dielectric = 0.04;

    vec3 F0 = vec3(dielectric);
    F0 = mix(F0, m_params.albedo, m_params.metallic);

    vec3 lightContribution = lighting(F0);
    vec3 IBLContribution = IBL(F0);

    color = vec4((m_params.emission + lightContribution + IBLContribution) * m_params.ao, 1.0);

    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    brightColor = brightness > 1.0 ? vec4(color.rgb, 1.0) : vec4(0.0, 0.0, 0.0, 1.0);
}