#version 330 core

in vec3 fragPos;
in vec3 objPos;

uniform float maxDayNight;
uniform float dayNight;
uniform samplerCube tex;

out vec4 color;

void main (void) {
	float tmp = -1.0 + 1.2 * (dayNight / maxDayNight);
	color = texture(tex, objPos) + vec4(tmp, tmp, tmp, 0.0f);
}
