#version 450

in vec3 position;
in vec3 normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec3 color;

out vec3 pass_color;
out vec3 pass_normal;

void main() {
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
	pass_color = color;	
	pass_normal = normal;
}

// #version 450

// in vec3 position;
// //uniform mat4 translationMatrix;
// //uniform mat4 rotationMatrix;
// //uniform mat4 scaleMatrix;
// uniform mat4 modelMatrix;
// uniform mat4 viewMatrix;
// uniform mat4 projectionMatrix;

// in vec3 color;

// out vec3 pass_color;

// void main() {
// 	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
// 	pass_color = color;
// }