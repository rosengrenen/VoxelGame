#version 420 core

in vec4 outColor;
in vec3 outNormal;
in vec3 outFragPos;

out vec4 FragColor;

void main()
{
    vec3 lightPos = vec3(400.0, 2000.0, 400.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(outNormal);
    vec3 lightDir = normalize(lightPos - outFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec4 result = vec4(ambient + diffuse, 1.0) * outColor;
    FragColor = result;
}