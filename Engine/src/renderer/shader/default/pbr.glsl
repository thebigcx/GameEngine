#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out DATA
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} vs_out;

layout (std140, binding = 0) uniform matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 transform = mat4(1.f);

void main()
{
    vs_out.normal = mat3(transform) * aNormal;
    vs_out.texCoord = aTexCoord;
    vs_out.fragPos = vec3(transform * vec4(aPos, 1.0));

    gl_Position = projection * view * vec4(vs_out.fragPos, 1.0);
}

#shader fragment

#define MAX_LIGHTS 64

struct Material
{
    float metallicScalar;
    float roughnessScalar;

    int textureFlags;

    vec3 albedoColor;
};

layout (binding = 0) uniform sampler2D materialAlbedo;
layout (binding = 1) uniform sampler2D materialNormal;
layout (binding = 2) uniform sampler2D materialMetallic;
layout (binding = 3) uniform sampler2D materialRoughness;
layout (binding = 4) uniform sampler2D materialAo;
layout (binding = 5) uniform sampler2D materialDepth;

layout (binding = 6) uniform samplerCube irradianceMap;
layout (binding = 7) uniform samplerCube prefilterMap;
layout (binding = 8) uniform sampler2D brdfLUT;

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

in DATA
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} fs_in;

out vec4 fragColor;

uniform vec3 cameraPos;

uniform Material material;

uniform PointLight pointLights[MAX_LIGHTS];
uniform int numPointLights;

uniform DirectionalLight directionalLight;
uniform int usingDirectionalLight;

uniform SkyLight skyLight;

uniform float exposure;

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

vec3 getNormalFromNormalMap(vec2 texCoord)
{
    /*
    vec3 normal = texture(materialNormal, texCoord).rgb;
    normal = normal * 2.0 - 1.0;
    return normal;
    */
    vec3 tangentNormal;
    if ((material.textureFlags & (1 << 1)) != 0)
        tangentNormal = texture(materialNormal, texCoord).xyz * 2.0 - 1.0;
    else
        return normalize(fs_in.normal);

    vec3 Q1 = dFdx(fs_in.fragPos);
    vec3 Q2 = dFdy(fs_in.fragPos);
    vec2 uv1 = dFdx(texCoord);
    vec2 uv2 = dFdy(texCoord);

    vec3 N = normalize(fs_in.normal);
    vec3 T = normalize(Q1 * uv1.s - Q2 * uv2.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec2 parallaxMapping(vec2 texCoord, vec3 viewDir)
{
    const float height_scale = 0.1;

    const float minLayers = 8;
    const float maxLayers = 32;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy / viewDir.z * height_scale;
    vec2 deltaTexCoord = P / numLayers;

    vec2 currentTexCoord = texCoord;
    float currentDepthMapValue = 1.0 - texture(materialDepth, currentTexCoord).r;

    while (currentLayerDepth < currentDepthMapValue)
    {
        currentTexCoord -= deltaTexCoord;
        currentDepthMapValue = 1.0 - texture(materialDepth, currentTexCoord).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoord = currentTexCoord + deltaTexCoord;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1.0 - texture(materialDepth, prevTexCoord).r - currentLayerDepth + layerDepth;

    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoord = prevTexCoord * weight + currentTexCoord * (1.0 - weight);

    return finalTexCoord;
}

// TODO: tangent space mapping in vertex shader
void main()
{
    vec3 V = normalize(cameraPos - fs_in.fragPos);

    vec2 texCoord = fs_in.texCoord;
    if ((material.textureFlags & (1 << 5)) != 0)
    {
        texCoord = parallaxMapping(fs_in.texCoord, V);
    }

    vec3 albedo;
    if ((material.textureFlags & (1 << 0)) != 0)
    {
        vec4 albedoSample = texture(materialAlbedo, texCoord);
        albedo = vec3(pow(albedoSample.r, 2.2), pow(albedoSample.g, 2.2), pow(albedoSample.b, 2.2));
    }
    else
    {
        albedo = material.albedoColor;
    }

    float metallic = 0.f;
    if ((material.textureFlags & (1 << 2)) != 0)
    {
        metallic = texture(materialMetallic, texCoord).r;
    }

    metallic = min(metallic + material.metallicScalar, 1.0);

    float roughness = 0.f;
    if ((material.textureFlags & (1 << 3)) != 0)
    {
        roughness = texture(materialRoughness, texCoord).r;
    }

    roughness = min(roughness + material.roughnessScalar, 1.0);

    float ao = 1.f;
    if ((material.textureFlags & (1 << 4)) != 0)
    {
        ao = texture(materialAo, texCoord).r;
    }
    
    vec3 N = getNormalFromNormalMap(texCoord);
    vec3 R = reflect(-V, N);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 fragPos = fs_in.fragPos;

    vec3 Lo = vec3(0);

    // ----------------------------Point Lights-------------------------------------

    for (int i = 0; i < numPointLights; i++)
    {
        vec3 lightPos = pointLights[i].position;

        vec3 L = normalize(lightPos - fragPos);
        vec3 H = normalize(V + L);

        float distance = length(lightPos - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].radiance * attenuation;

        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;

        Lo *= pointLights[i].intensity;
    }
    
    // ----------------------------Directional Light-------------------------------------
    if (usingDirectionalLight == 1)
    {
        vec3 L = normalize(-directionalLight.direction);
        vec3 H = normalize(V + L);

        vec3 radiance = directionalLight.radiance;

        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;

        Lo *= directionalLight.intensity;
    }

    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

    vec3 mapped = vec3(1.0) - exp(-color * exposure);

    mapped = pow(mapped, vec3(1.0 / 2.2));

    fragColor = vec4(mapped, 1.0);
}