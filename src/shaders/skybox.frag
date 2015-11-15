#version 330 core

in vec3 fragPos;
in vec3 objPos;
uniform samplerCube tex;

out vec4 color;

void main (void) {
	color = texture(tex, objPos);
}
