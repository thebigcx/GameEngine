#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

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
    vs_out.normal = aNormal;
    vs_out.texCoord = aTexCoord;
    vs_out.fragPos = vec3(transform * vec4(aPos, 1.0));

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
};

struct PointLight
{
    vec3 position;
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

//uniform vec3 lightPositions[MAX_LIGHTS];
//uniform vec3 lightColors[MAX_LIGHTS];

uniform float skyLight;

float PI = 3.1415926535897932384626433832795028841971693993751058209749445923;

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{
    vec3 N = normalize(fs_in.normal);
    vec3 V = normalize(cameraPos - fs_in.fragPos);

    vec4 albedoSample = texture(material.albedo, fs_in.texCoord);

    vec3 albedo = vec3(pow(albedoSample.r, 2.2), pow(albedoSample.g, 2.2), pow(albedoSample.b, 2.2));
    //vec3 normal = getNormalFromNormalMap();
    float metallic  = texture(material.metallic, fs_in.texCoord).r;
    float roughness = texture(material.roughness, fs_in.texCoord).r;
    float ao        = texture(material.ao, fs_in.texCoord).r;

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0);
    for (int i = 0; i < numPointLights; i++)
    {
        vec3 L = normalize(pointLights[i].position - fs_in.fragPos);
        vec3 H = normalize(V + L);

        float distance = length(pointLights[i].position - fs_in.fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = pointLights[i].radiance * attenuation;

        float NDF = distributionGGX(H, V, roughness);
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
    }

    vec3 ambient = skyLight * albedo * ao;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    fragColor = vec4(color, 1.0);
}