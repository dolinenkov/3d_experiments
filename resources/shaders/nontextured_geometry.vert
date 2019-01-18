#version 330 core

uniform mat4 u_ModelViewProjectionMatrix;

in vec3 a_Position;

void main()
{
    gl_Position = u_ModelViewProjectionMatrix * vec4(a_Position, 1.0);
}
