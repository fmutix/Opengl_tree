#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform vec3 objectPos;
uniform float objectScale;

layout (location = 0) in vec3 position;

out vec3 objPos;

void main() {
	gl_Position = projection * view * world * vec4(position * objectScale + objectPos, 1.0f);
	objPos = position;
}
