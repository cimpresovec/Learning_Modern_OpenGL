#version 150

in vec3 draw_colors;

out vec4 out_colors;

uniform float uni_red;

void main()
{
	out_colors = vec4(draw_colors.r * uni_red, draw_colors.g, draw_colors.b, 1.0);
}