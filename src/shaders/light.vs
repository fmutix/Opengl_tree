	#version 330 core

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;
uniform vec3 objectModel;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragNormal;
out vec3 fragPos;
out vec3 objPos;

void main() {
	gl_Position = projection * view * world * vec4(position + objectModel, 1.0f);
	fragPos = vec3(world * vec4(position + objectModel, 1.0f));
	objPos = position;

	fragNormal = normalize(
		mat3(transpose(inverse(world))) * normal
	);
}
