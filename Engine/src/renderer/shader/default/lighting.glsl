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
    //vec3 cameraPos;
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
    sampler2D specular;

    vec4 albedoColor;

    float roughness;
    float metalness;
};

struct DirLight
{
    vec3 direction;
    vec3 color;
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

in DATA
{
    vec2 texCoord;
    vec3 normal;
    vec3 fragPos;
} fs_in;

out vec4 FragColor;

uniform Material material;

/*layout (std140, binding = 1) uniform lighting
{
    DirLight dirLightTest;
    //PointLight sfsfdg[MAX_LIGHTS];
    //SpotLight sgdfsgsgsds[MAX_LIGHTS];

    //int sgdhtrtsg;
    //int shrdgsrhg;

    vec3 cameraPos;
    float skyLight;
};*/

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_LIGHTS];
uniform SpotLight spotLights[MAX_LIGHTS];

uniform int numPointLights;
uniform int numSpotLights;

uniform float skyLight;
uniform vec3 cameraPos;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
LightValues calculateLight(vec3 lightDir, vec3 normal, vec3 viewDir, vec3 lightDiffuse, vec3 lightSpecular);

void main()
{
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(cameraPos - fs_in.fragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);

    result += skyLight * vec3(texture(material.albedo, fs_in.texCoord));
    
    for (int i = 0; i < numPointLights; i++)
        result += calcPointLight(pointLights[i], norm, fs_in.fragPos, viewDir);

    for (int i = 0; i < numSpotLights; i++)
        result += calcSpotLight(spotLights[i], norm, fs_in.fragPos, viewDir);

    //FragColor = vec4(result, 1.0);
    FragColor = vec4(texture(material.albedo, fs_in.texCoord) * material.albedoColor);
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.roughness);

    values.diffuse  = lightDiffuse  * diff * vec3(texture(material.albedo, fs_in.texCoord) * material.albedoColor);
    values.specular = lightSpecular * spec * vec3(texture(material.specular, fs_in.texCoord));

    return values;
}