#shader vertex

#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view = mat4(1.f);
uniform mat4 transform = mat4(1.f);

void main()
{
    Normal = aNormal;
    TexCoord = aTexCoord;
    FragPos = vec3(transform * vec4(aPos, 1.0));
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
}

#shader fragment

#version 460 core

#define MAX_LIGHTS 64

struct Material
{
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight
{
    vec3 direction, color;
    float intensity;
    float specular;
};

struct PointLight
{
    vec3 position, color;
    float intensity;
    float specular;
    float attenuation;
};

struct SpotLight
{
    vec3 position, direction, color;

    float cutoff, outerCutoff;

    float intensity;
    float specular;
    float attenuation;
};

struct LightValues
{
    vec3 ambient, diffuse, specular;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

uniform int numPointLights = 0;
uniform int numSpotLights = 0;

uniform vec3 viewPos;
uniform float skyLight;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
LightValues calculateLight(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightDiffuse, vec3 lightSpecular);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);

    result += skyLight * vec3(texture(material.diffuse, TexCoord));
    
    for (int i = 0; i < numPointLights; i++)
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir);

    for (int i = 0; i < numSpotLights; i++)
        result += calcSpotLight(spotLights[i], norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    LightValues values = calculateLight(lightDir, normal, viewDir, light.color * light.intensity, light.color * light.specular);

    return (values.ambient + values.diffuse + values.specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    LightValues values = calculateLight(lightDir, normal, viewDir, light.color * light.intensity, light.color * light.specular);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1 + light.attenuation * (distance * distance));

    values.ambient *= attenuation;
    values.diffuse *= attenuation;
    values.specular *= attenuation;

    return (values.ambient + values.diffuse + values.specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    LightValues values = calculateLight(lightDir, normal, viewDir, light.color * light.intensity, light.color * light.specular);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

    values.diffuse *= intensity;
    values.specular *= intensity;

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (1.f + light.attenuation * (distance * distance));   

    values.diffuse *= attenuation;
    values.specular *= attenuation;

    return (values.ambient + values.diffuse + values.specular);
}

LightValues calculateLight(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightDiffuse, vec3 lightSpecular)
{
    LightValues values;

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    values.diffuse  = lightDiffuse  * diff * vec3(texture(material.diffuse, TexCoord));
    values.specular = lightSpecular * spec * vec3(texture(material.specular, TexCoord));

    return values;
}