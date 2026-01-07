#version 410 core

in vec3 FragPos;
in vec3 Normal;
in vec3 vColor;

//uniform vec3 viewPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightInt;

out vec4 FragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    //vec3 viewDir  = normalize(viewPos - FragPos);

    vec3 ambient = vec3(0.4);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * lightInt;

    vec3 col = (ambient + diffuse) * vColor;
    FragColor = vec4(col, 1.0); // color
}
