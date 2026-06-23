#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec3 specColor;
layout (location = 4) in float shininess;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec3 vColor;
out vec3 vSpecColor;
out float vShininess;
out vec4 FragPosLightSpace;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = perspective * view * model * vec4(aPos, 1.0);
    vColor = aColor;
    vSpecColor = specColor;
    vShininess = shininess;

    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
}


