#version 330 core

uniform sampler2D   u_Texture;
uniform float       u_Gamma;

in vec3 v_Texture;

out vec4 o_Color;

void main()
{
    o_Color = texture(u_Texture, v_Texture.st);

    o_Color.rgb = pow(o_Color.rgb, vec3(1.0 / u_Gamma));
}
