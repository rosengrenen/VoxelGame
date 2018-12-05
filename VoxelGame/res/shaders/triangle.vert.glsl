#version 420 core

in vec3 inPos;

uniform mat4 model, view, projection;

void main()
{
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}