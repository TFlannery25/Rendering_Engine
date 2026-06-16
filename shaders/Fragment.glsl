#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;
in vec4 FragPosLightSpace;

uniform vec3 viewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightInt;
uniform sampler2D shadowMap;

out vec4 FragColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // convert from clip space (-1 to 1) to texture space (0 to 1)
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 ambient = vec3(.6);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightInt;

    float specStrength = 0.5;
    float shininess = 15.0;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specStrength * spec * lightColor;

    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 col = (ambient + (1.0 - shadow) * (diffuse + specular)) * vColor;
    FragColor = vec4(col, 1.0); // color
}
