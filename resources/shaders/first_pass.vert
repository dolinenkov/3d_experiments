#version 330 core


uniform mat4 u_ProjectionMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ModelMatrix;


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

    v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;

    v_Texture = a_Texture;

    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPosition;
}
