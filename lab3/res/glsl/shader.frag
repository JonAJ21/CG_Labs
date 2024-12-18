#version 450

out vec4 color;

in vec3 pass_color;
in vec3 pass_normal;
// uniform float animation;

void main() {
    vec3 lightDir = normalize(vec3(2.0, 2.0, 2.0));
    float diff = max(dot(pass_normal, lightDir), 0.0);
    vec3 diffuse = diff * pass_color;
    
    vec3 ambient = 0.1 * pass_color;
    
    color = vec4(ambient + diffuse, 1.0);
	// color = vec4(pass_color, 1);
}

// #version 450

// out vec4 color;

// in vec3 pass_color;

// uniform float animation;

// void main() {
// 	//color = vec4(pass_color * animation, 1);
// 	color = vec4(pass_color, 1);
// }