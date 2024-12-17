#version 450

in vec3 position;
//uniform mat4 translationMatrix;
//uniform mat4 rotationMatrix;
//uniform mat4 scaleMatrix;
uniform mat4 modelMatrix;

in vec3 color;

out vec3 pass_color;

void main() {
	gl_Position = modelMatrix * vec4(position, 1);
	pass_color = color;
}