#version 330 core

uniform vec3 u_Color;

out vec4 o_FragmentColor;

void main()
{
    o_FragmentColor = vec4(u_Color, 1.0);
}
