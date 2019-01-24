#version 330 core


uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;
uniform mat3 u_NormalMatrix;


in vec3 a_Position;
in vec3 a_Normal;
in vec3 a_Texture;

out vec3 v_Position;
out vec3 v_Normal;
out vec3 v_Texture;

void main()
{
    vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0);

    v_Position = vec3(worldPosition);

    v_Normal = u_NormalMatrix * a_Normal;

    v_Texture = a_Texture;

    gl_Position = u_ViewProjectionMatrix * worldPosition;
}
