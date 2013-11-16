#version 150

out vec4 colors;
uniform float red_changer;

void main()
{
	colors = vec4(1.0*red_changer, 0.0, 0.0, 1.0);
}