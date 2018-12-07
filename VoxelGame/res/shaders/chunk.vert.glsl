#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out vec4 outColor;
out vec3 outNormal;
out vec3 outFragPos;

uniform mat4 model, view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    outColor = color;
    outNormal = normal;
    outFragPos = vec3(model * vec4(position, 1.0));
}