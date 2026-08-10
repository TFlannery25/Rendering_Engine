#version 410 core

#define MAX_LIGHTS 4
#define MAX_SHADOW_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;
in vec3 vSpecColor;
in float vShininess;

uniform vec3 viewPos;

struct Light
{
    vec3 position;
    vec3 color;
    float intensity;
};

uniform Light illuminationLights[MAX_LIGHTS];
uniform int numIlluminationLights;

uniform Light shadowLights[MAX_SHADOW_LIGHTS];
uniform mat4 lightSpaceMatrices[MAX_SHADOW_LIGHTS];
uniform sampler2D shadowMaps[MAX_SHADOW_LIGHTS];
uniform int numShadowLights;

out vec4 FragColor;

float ShadowCalculation(vec3 fragPos, mat4 lightSpaceMatrix, sampler2D shadowMap)
{
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 ComputeLightContribution(Light light, vec3 norm, vec3 viewDir, float shadow)
{
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), vShininess);
    vec3 specular = vSpecColor * spec * light.color;

    return (1.0 - shadow) * (diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = vec3(.1);
    vec3 result = ambient;

    for (int i = 0; i < numIlluminationLights; i++)
    {
        result += ComputeLightContribution(illuminationLights[i], norm, viewDir, 0.0);
    }

    for (int i = 0; i < numShadowLights; i++)
    {
        float shadow = ShadowCalculation(FragPos, lightSpaceMatrices[i], shadowMaps[i]);
        result += ComputeLightContribution(shadowLights[i], norm, viewDir, shadow);
    }

    vec3 col = result * vColor;
    FragColor = vec4(col, 1.0);
}