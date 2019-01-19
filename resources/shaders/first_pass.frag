#version 330 core


uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;

uniform sampler2D u_DiffuseTexture;

in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Texture;

out vec4 o_FragmentColor;


float diffuseIntensity(const vec3 position, const vec3 normal)
{
    return max(dot(normalize(normal), normalize(u_LightPosition - position)), 0.0);
}

float ambientIntensity()
{
    return 0.1;
}


void main()
{
    vec3 color = u_LightColor * texture(u_DiffuseTexture, v_Texture.st).rgb * (diffuseIntensity(v_Position, v_Normal) + ambientIntensity());
//    vec3 color = vec3(1.0, 1.0, 1.0) * diffuseIntensity(v_Position, vec3(0.0, 0.0, -1.0));

    o_FragmentColor = vec4(color, 1.0) ;
}
