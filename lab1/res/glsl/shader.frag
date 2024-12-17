#version 450

out vec4 color;

in vec3 pass_color;

uniform float animation;

void main() {
	//color = vec4(pass_color * animation, 1);
	color = vec4(pass_color, 1);
}