#version 330 core

in vec3 fragPos;
in vec3 objPos;
uniform sampler2D tex;

out vec4 color;

void main (void) {
	color = texture(tex, vec2(objPos.y, objPos.z));
}
