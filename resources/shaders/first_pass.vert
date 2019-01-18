#version 330 core

uniform mat4 u_MatrixModelViewProjection;

in vec3 a_Position;
in vec3 a_Texture;

out vec3 v_Texture;

void main()
{
    gl_Position = u_MatrixModelViewProjection * vec4(a_Position, 1.0);
    v_Texture = a_Texture;
}
