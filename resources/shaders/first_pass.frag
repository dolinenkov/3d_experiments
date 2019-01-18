#version 330 core

uniform sampler2D u_DiffuseTexture;

in vec3 v_Texture;

out vec4 o_FragmentColor;

void main()
{
    o_FragmentColor = texture(u_DiffuseTexture, v_Texture.xy);
}
