#version 330 core

uniform mat4 u_MatrixModelViewProjection;
uniform mat4 u_MatrixModel;
uniform mat3 u_MatrixNormal;

in vec3 a_Position;
in vec3 a_Normal;
in vec3 a_Texture;

out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Texture;

void main()
{
    gl_Position = u_MatrixModelViewProjection * vec4(a_Position, 1.0);
    v_Position = (u_MatrixModel * vec4(a_Position, 1.0)).xyz;
    v_Normal = u_MatrixNormal * a_Normal;
    v_Texture = a_Texture;
}
