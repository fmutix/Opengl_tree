#version 330 core

uniform vec3 objectColor;

in vec3 fragPos;
in vec3 objPos;
in vec3 fragNormal;

out vec4 color;

void main (void) {
	color = vec4(objectColor, 1.0);
}
