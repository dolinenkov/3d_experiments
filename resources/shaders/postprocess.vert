#version 330 core

in vec3 a_Position;
in vec3 a_Texture;

out vec3 v_Texture;

void main()
{
    v_Texture = a_Texture;
    gl_Position = vec4(a_Position.xy, 0.0, 1.0);
}
