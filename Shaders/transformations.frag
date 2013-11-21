#version 150

in vec3 color;
in vec2 tex_coord;

out vec4 out_color;

uniform sampler2D tex;

void main()
{
	out_color = texture(tex, tex_coord) * vec4(color, 0.7f);
}