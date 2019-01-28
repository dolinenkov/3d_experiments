#version 330 core

uniform sampler2D   u_Texture;
uniform float       u_Gamma;
uniform float       u_Exposure;

in vec3 v_Texture;

out vec4 o_Color;


void main()
{
    vec4 inColor = texture(u_Texture, v_Texture.st);

    vec3 toneMappedColor = vec3(1.0) - exp(inColor.rgb * -u_Exposure);

    vec3 gammaCorrectedColor = pow(toneMappedColor, vec3(1.0 / u_Gamma));

    o_Color = vec4(gammaCorrectedColor, inColor.a);
}
